#include "CanvasRenderer.h"

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

void CanvasRenderer::renderCanvas()
{
	bool isPrevAvailable = m_pSettings->previousAlpha && m_currentFrame - 1 >= 0;
	double origCoeff = getOriginalCoeff();

	SDL_Texture* t = SDL_CreateTextureFromSurface(m_pRenderer, canvas()->m_pSurface);

	auto size = getSizeInWindow();
	SDL_Rect copyRect = {
		0, 0,
		size.x, size.y,
	};

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

			SDL_Texture* t = SDL_CreateTextureFromSurface(m_pRenderer, prevCanvas->m_pSurface);
			SDL_RenderCopy(m_pRenderer, t, nullptr, &copyRect);

			SDL_SetRenderDrawColor(m_pRenderer, 32, 32, 32, 255);
			SDL_RenderDrawRect(m_pRenderer, &copyRect);
		}
	}
}

void CanvasRenderer::render()
{
	renderCanvas();
	renderGrid();
	renderMinimap();
	if (m_pSettings->previousMap)
		renderPrevious();
}

CanvasRenderer::CanvasRenderer(Settings* settings, SDL_Renderer* renderer, size_t w, size_t h) 
	: m_pSettings(settings), m_pRenderer(renderer), m_winWidth(w), m_winHeight(h)
{
	m_frames.reserve(32);
	m_frames.push_back(new Canvas(m_width, m_height));
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

	render();

	if (e.type == SDL_MOUSEBUTTONDOWN)
		m_isMousePressed = true;
	else if (e.type == SDL_MOUSEBUTTONUP)
		m_isMousePressed = false;

	bool isInsideX = e.motion.x >= 0 && e.motion.x < canvas()->getWidth() * origCoeff + menuBarOffset;
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

