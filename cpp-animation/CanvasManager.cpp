#include "CanvasManager.h"

CanvasManager::CanvasManager(Settings* settings) : m_pSettings(settings)
{
	m_pAnimationManager = new AnimationManager(m_pSettings);
	m_pRenderer = nullptr; // need to call createRenderer
}

CanvasRenderer* CanvasManager::getRenderer()
{
	return m_pRenderer;
}

AnimationManager* CanvasManager::getAnimationManager()
{
	return m_pAnimationManager;
}

Animation* CanvasManager::getAnimation()
{
	return m_pAnimationManager->getAnimation();
}

void CanvasManager::createRenderer(SDL_Renderer* renderer, size_t winWidth, size_t winHeight)
{
	m_pRenderer = new CanvasRenderer(m_pSettings, renderer, m_pAnimationManager->getAnimation(), winWidth, winHeight);
}

void CanvasManager::reload(int w, int h)
{
	m_pAnimationManager->reload(w, h);
	m_pSettings->zoom = 1;
	m_pSettings->isAnimating = false;
	m_pRenderer->reload(m_pAnimationManager->getAnimation());
}

bool CanvasManager::keyWasPressed(SDL_Event& e, SDL_Scancode key)
{
	return e.type == SDL_KEYDOWN && e.key.keysym.scancode == key;
}

void CanvasManager::updateWindowSize(int w, int h)
{
	m_pRenderer->updateWindowSize(w, h);
}

void CanvasManager::update(SDL_Event& e)
{
	m_pAnimationManager->update(e);
	m_pRenderer->update(e);

	if (m_pSettings->isAnimating)
		return; // to doesn't interact while animation

	if (e.type == SDL_MOUSEBUTTONDOWN)
		m_isMousePressed = true;
	else if (e.type == SDL_MOUSEBUTTONUP)
		m_isMousePressed = false;


	if (
		m_pRenderer->isInsideCanvas(e.motion.x, e.motion.y) && (
			e.type == SDL_MOUSEBUTTONDOWN || (e.type == SDL_MOUSEMOTION && m_isMousePressed)
			)
		)
	{
		auto coords = m_pRenderer->getCanvasCoords(e.motion.x, e.motion.y);
		auto offset = m_pRenderer->getOffset();
		SDL_Color color = m_pAnimationManager->getCurrentColor();
		m_pAnimationManager->getAnimation()->actOnCanvas(
			coords.x + offset.x, coords.y + offset.y, color
		);
	}

	if (keyWasPressed(e, SDL_SCANCODE_Z))
		m_pRenderer->zoom(true);
	else if (keyWasPressed(e, SDL_SCANCODE_X))
		m_pRenderer->zoom(false);

	if (m_pSettings->zoom > 1)
	{
		auto coords = m_pRenderer->getCanvasCoords(e.motion.x, e.motion.y);
		auto offset = m_pRenderer->getOffset();
		auto block = m_pRenderer->getSizeOfBlock();
		Canvas* canvas = m_pAnimationManager->getAnimation()->getCanvas();

		int shift = m_pSettings->maxZoom / m_pSettings->zoom;
		int remainsX = canvas->getWidth() - offset.x - shift - block.x;
		int remainsY = canvas->getHeight() - offset.y - shift - block.y;

		if (keyWasPressed(e, SDL_SCANCODE_UP) && offset.y - shift >= 0) m_pRenderer->shift(0, -shift);
		else if (keyWasPressed(e, SDL_SCANCODE_DOWN) && remainsY >= 0) m_pRenderer->shift(0, shift);
		if (keyWasPressed(e, SDL_SCANCODE_LEFT) && offset.x - shift >= 0) m_pRenderer->shift(-shift, 0);
		else if (keyWasPressed(e, SDL_SCANCODE_RIGHT) && remainsX >= 0) m_pRenderer->shift(shift, 0);
	}
}
