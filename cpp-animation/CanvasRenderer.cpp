#include "CanvasRenderer.h"


Uint32 animationCallback(Uint32 interval, void* param)
{
	Settings* settings = (Settings*)param;

	if (settings->isAnimating)
	{
		SDL_Event e;
		SDL_UserEvent userEvent;

		userEvent.type = SDL_USEREVENT;
		userEvent.code = 0;
		userEvent.data1 = nullptr;
		userEvent.data2 = nullptr;

		e.type = SDL_USEREVENT;
		e.user = userEvent;

		SDL_PushEvent(&e);
	}

	return interval;
}

void pushAnimation(Settings* settings)
{ 
	SDL_RemoveTimer(settings->lastTimerId);
	settings->lastTimerId = SDL_AddTimer((33 / 10) * 50 * settings->delay, animationCallback, settings);
}


Vector<double> CanvasRenderer::getSizeOfBlock()
{
	return {
		canvas()->getWidth() / double(m_pSettings->zoom),
		canvas()->getHeight() / double(m_pSettings->zoom)
	};
}

double CanvasRenderer::getOriginalCoeff()
{
	double coeffW = double(m_winWidth) / canvas()->m_pSurface->w;
	double coeffH = double(m_winHeight) / canvas()->m_pSurface->h;
	return min(coeffW, coeffH);
}

double CanvasRenderer::getCoeff()
{
	return getOriginalCoeff() * m_pSettings->zoom;
}

Vector<double> CanvasRenderer::getCanvasCoords(int x, int y)
{
	double coeff = getCoeff();

	return {
		x / coeff,
		(y - menuBarOffset) / coeff,
	};
}

Vector<int> CanvasRenderer::getOffset()
{
	return {
		m_offsetX * (m_pSettings->zoom == 1 ? 0 : 1),
		m_offsetY * (m_pSettings->zoom == 1 ? 0 : 1)
	};
}

bool CanvasRenderer::keyWasPressed(SDL_Event& e, SDL_Scancode s)
{
	return e.type == SDL_KEYDOWN && e.key.keysym.scancode == s;
}

Canvas* CanvasRenderer::canvas()
{
	return m_frames[m_currentFrame];
}

void CanvasRenderer::createBackground()
{
	m_pBackgroundCanvas = new Canvas(2, 2);

	SDL_Rect microRect = {
		0, 0,
		1, 1
	};
	SDL_Color color = { 0, 0, 0, 255 };

	for (int y = 0; y < m_pBackgroundCanvas->getHeight(); ++y)
	{
		int k = y % 2 != 0; // offset for odd rows
		for (int x = 0; x < m_pBackgroundCanvas->getWidth(); ++x)
		{
			if (k % 2 == 0)
				color = { 221, 221, 221, 255 };
			else
				color = { 255, 255, 255, 255 };
			m_pBackgroundCanvas->setRect(microRect, color);
			++k;
			microRect.x += 1;
		}
		microRect.x = 0;
		microRect.y += 1;
	}
}

void CanvasRenderer::renderBackground(SDL_Rect& rect)
{
	double coeff = double(rect.w) / canvas()->getWidth();
	SDL_Texture* t = SDL_CreateTextureFromSurface(m_pRenderer, m_pBackgroundCanvas->m_pSurface);

	auto size = getSizeInWindow();
	SDL_FRect copyRect = {
		rect.x, rect.y,
		coeff, coeff,
	};
	for (int y = 0; y < canvas()->getHeight(); ++y)
	{
		for (int x = 0; x < canvas()->getWidth(); ++x)
		{
			copyRect.x = coeff * x + rect.x;
			copyRect.y = coeff * y + rect.y;
			SDL_RenderCopyF(m_pRenderer, t, nullptr, &copyRect);
		}
	}
}

void CanvasRenderer::renderCanvas(Canvas* c, bool checkPrev)
{
	auto size = getSizeInWindow();
	SDL_Rect copyRect = {
		0, 0,
		size.x, size.y,
	};
	renderBackground(copyRect);

	bool isPrevAvailable = checkPrev && m_pSettings->previousAlpha && m_currentFrame - 1 >= 0;
	double origCoeff = getOriginalCoeff();

	SDL_Texture* t = SDL_CreateTextureFromSurface(m_pRenderer, c->m_pSurface);

	auto blockSize = getSizeOfBlock();
	auto offset = getOffset();

	SDL_Rect zoomRect = {
		offset.x, offset.y,
		blockSize.x,
		blockSize.y
	};
	SDL_RenderCopy(m_pRenderer, t, &zoomRect, &copyRect);
	if (isPrevAvailable)
	{
		Canvas* prevCanvas = m_frames[m_currentFrame - 1];
		SDL_Texture* prev = SDL_CreateTextureFromSurface(m_pRenderer, prevCanvas->m_pSurface);
		SDL_SetTextureAlphaMod(prev, m_pSettings->alpha);
		SDL_RenderCopy(m_pRenderer, prev, &zoomRect, &copyRect);
	}
}

void CanvasRenderer::renderGrid()
{
	SDL_SetRenderDrawColor(m_pRenderer, 32, 32, 32, 255);
	double origCoeff = getOriginalCoeff();
	double coeff = getCoeff();

	auto block = getSizeOfBlock();
	auto size = getSizeInWindow();

	for (size_t i = 1; i <= block.x; i++)
		SDL_RenderDrawLineF(m_pRenderer, i * coeff, 0, i * coeff, size.x - 1);

	for (size_t i = 1; i <= block.y; i++)
		SDL_RenderDrawLineF(m_pRenderer, 0, i * coeff, size.y - 1, i * coeff);
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
}

void CanvasRenderer::renderMinimap()
{
	double origCoeff = getOriginalCoeff();

	auto size = getSizeInWindow();
	int mapSize = (m_winWidth - size.x);
	if (mapSize > m_winHeight / 2)
		mapSize /= 2;
	double mapCoeff = double(mapSize) / canvas()->getWidth();
	if (mapSize > 0)
	{
		SDL_Rect copyRect = {
			size.x, 0,
			mapSize, mapSize,
		};
		renderBackground(copyRect);

		SDL_Texture* t = SDL_CreateTextureFromSurface(m_pRenderer, canvas()->m_pSurface);
		SDL_RenderCopy(m_pRenderer, t, nullptr, &copyRect);

		SDL_SetRenderDrawColor(m_pRenderer, 32, 32, 32, 255);
		SDL_RenderDrawRect(m_pRenderer, &copyRect);

		int zoomSize = mapSize / m_pSettings->zoom;
		auto offset = getOffset();

		SDL_Rect zoomAreaRect = {
			size.x + offset.x * mapCoeff, offset.y * mapCoeff,
			zoomSize, zoomSize
		};
		SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(m_pRenderer, &zoomAreaRect);
	}
}

void CanvasRenderer::renderPrevious()
{
	if (m_currentFrame - 1 >= 0)
	{
		Canvas* prevCanvas = m_frames[m_currentFrame - 1];
		double origCoeff = getOriginalCoeff();
		auto size = getSizeInWindow();

		int mapSize = (m_winWidth - size.x);
		if (mapSize > m_winHeight / 2)
			mapSize /= 2;

		if (mapSize > 0)
		{
			SDL_Rect copyRect = {
				size.x, mapSize - 1,
				mapSize, mapSize,
			};
			renderBackground(copyRect);

			SDL_Texture* t = SDL_CreateTextureFromSurface(m_pRenderer, prevCanvas->m_pSurface);
			SDL_RenderCopy(m_pRenderer, t, nullptr, &copyRect);

			SDL_SetRenderDrawColor(m_pRenderer, 32, 32, 32, 255);
			SDL_RenderDrawRect(m_pRenderer, &copyRect);
		}
	}
}

void CanvasRenderer::render()
{
	renderCanvas(canvas(), true);
	renderGrid();
	if (!m_pSettings->isAnimating && m_pSettings->previousMap)
		renderPrevious();
	renderMinimap();
	
}

CanvasRenderer::CanvasRenderer(Settings* settings, SDL_Renderer* renderer, size_t w, size_t h) 
	: m_pSettings(settings), m_pRenderer(renderer), m_winWidth(w), m_winHeight(h)
{
	m_frames.reserve(32);
	m_frames.push_back(new Canvas(m_width, m_height));
	createBackground();
}

void CanvasRenderer::recreate(int w, int h)
{
	m_width = w;
	m_height = h;

	m_frames.clear();
	m_frames.push_back(new Canvas(m_width, m_height));
	m_currentFrame = 0;

	m_pSettings->zoom = 1;
	m_offsetX = 0;
	m_offsetY = 0;
}

void CanvasRenderer::newFrame()
{
	m_frames.push_back(new Canvas(m_width, m_height));
	++m_currentFrame;
}

void CanvasRenderer::updateWindowSize(int w, int h)
{
	m_winWidth = w;
	m_winHeight = h;
}

void CanvasRenderer::update(SDL_Event& e)
{
	double origCoeff = getOriginalCoeff();
	double coeff = getCoeff();
	auto block = getSizeOfBlock();
	auto offset = getOffset();

	
	
	if (m_pSettings->isAnimating)
	{
		if (e.type == SDL_USEREVENT && e.user.code == 0)
			pushAnimation(m_pSettings); // update doesn't work without lots of events

		
		if (SDL_GetTicks() - lastTick > (33 / 10) * 100 * m_pSettings->delay)
		{
			lastTick = SDL_GetTicks64();
			m_currentFrame = (m_currentFrame + 1) % getFramesCount();
		}
	}

	render();

	if (m_pSettings->isAnimating)
		return; // to doesn't interact while animation

	if (e.type == SDL_MOUSEBUTTONDOWN)
		m_isMousePressed = true;
	else if (e.type == SDL_MOUSEBUTTONUP)
		m_isMousePressed = false;

	bool isInsideX = e.motion.x >= 0 && e.motion.x < canvas()->getWidth() * origCoeff;
	bool isInsideY = e.motion.y >= 0 && e.motion.y < canvas()->getHeight() * origCoeff + menuBarOffset;
	
	bool isCaptured = ImGui::IsWindowHovered(ImGuiFocusedFlags_AnyWindow) || ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);
	if (isInsideX && isInsideY && !isCaptured)
	{
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			auto coords = getCanvasCoords(e.motion.x, e.motion.y);
			canvas()->setPixel(
				coords.x + offset.x,
				coords.y + offset.y,
				Uint8(m_pSettings->color[0] * 255),
				Uint8(m_pSettings->color[1] * 255),
				Uint8(m_pSettings->color[2] * 255)
			);
		}

		if (e.type == SDL_MOUSEMOTION && m_isMousePressed)
		{
			auto coords = getCanvasCoords(e.motion.x, e.motion.y);
			canvas()->setPixel(
				coords.x + offset.x,
				coords.y + offset.y,
				Uint8(m_pSettings->color[0] * 255),
				Uint8(m_pSettings->color[1] * 255),
				Uint8(m_pSettings->color[2] * 255)
			);
		}

		if (e.type == SDL_MOUSEMOTION)
		{
			auto coords = getCanvasCoords(e.motion.x, e.motion.y);
			SDL_FRect select = {
				int(coords.x) * coeff,
				int(coords.y) * coeff,
				coeff + 1, coeff + 1
			};
			SDL_SetRenderDrawColor(m_pRenderer, 0xff, 0xff, 0xff, 0xff);
			SDL_RenderDrawRectF(m_pRenderer, &select);
		}
	}

	if (keyWasPressed(e, SDL_SCANCODE_Z) && m_pSettings->zoom < m_pSettings->maxZoom)
		m_pSettings->zoom *= 2;
	else if (keyWasPressed(e, SDL_SCANCODE_X) && m_pSettings->zoom > 1)
	{
		m_pSettings->zoom /= 2;
		if (m_pSettings->zoom > 1)
		{
			// сдвиг назад, чтоб не было искажённого квадрата, при запросе к канвасу
			auto newBlock = getSizeOfBlock();
			// делить на 2, потому что zoom изменился в 2 раза
			if (m_offsetX + newBlock.x > canvas()->getWidth() && m_offsetX - newBlock.x / 2 >= 0) m_offsetX -= newBlock.x / 2;
			if (m_offsetY + newBlock.y > canvas()->getHeight() && m_offsetY - newBlock.y / 2 >= 0) m_offsetY -= newBlock.y / 2;
		}
	}

	if (m_pSettings->zoom > 1)
	{
		int shift = m_pSettings->maxZoom / m_pSettings->zoom;
		int remainsX = canvas()->getWidth() - offset.x - shift - block.x;
		int remainsY = canvas()->getHeight() - offset.y - shift - block.y;
		
		if (keyWasPressed(e, SDL_SCANCODE_UP) && offset.y - shift >= 0) m_offsetY -= shift;
		else if (keyWasPressed(e, SDL_SCANCODE_DOWN) && remainsY >= 0) m_offsetY += shift;
		if (keyWasPressed(e, SDL_SCANCODE_LEFT) && offset.x - shift >= 0) m_offsetX -= shift;
		else if (keyWasPressed(e, SDL_SCANCODE_RIGHT) && remainsX >= 0) m_offsetX += shift;
	}
	
}

void CanvasRenderer::switchAnimation()
{
	lastTick = SDL_GetTicks64();
	m_pSettings->isAnimating = !m_pSettings->isAnimating;
}

void CanvasRenderer::save(Saver* saver)
{
	saver->save(m_frames, m_width, m_height);
}

size_t CanvasRenderer::getFramesCount()
{
	return m_frames.size();
}

Vector<int> CanvasRenderer::getSizeInWindow()
{
	double origCoeff = getOriginalCoeff();
	int width = origCoeff * canvas()->getWidth();
	int height = origCoeff * canvas()->getHeight();
	return {
		width, height
	};
}

