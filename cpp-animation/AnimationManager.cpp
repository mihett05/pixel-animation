#include "AnimationManager.h"

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


AnimationManager::AnimationManager(Settings* settings) : m_pSettings(settings)
{
	m_pAnimation = new Animation(defaultWidth, defaultHeight);
}

Animation* AnimationManager::getAnimation()
{
	return m_pAnimation;
}

SDL_Color AnimationManager::getCurrentColor()
{
	SDL_Color color = {
		255, 255, 255, 0
	};

	if (m_pSettings->tool == DrawTool::brush)
		color = {
			Uint8(m_pSettings->color[0] * 255),
			Uint8(m_pSettings->color[1] * 255),
			Uint8(m_pSettings->color[2] * 255),
			Uint8(m_pSettings->color[3] * 255)
	};

	return color;
}

void AnimationManager::update(SDL_Event& e)
{
	if (m_pSettings->isAnimating)
	{
		if (e.type == SDL_USEREVENT && e.user.code == 0)
			pushAnimation(m_pSettings); // update doesn't work without lots of events

		if (SDL_GetTicks() - lastTick > (33 / 10) * 100 * m_pSettings->delay)
		{
			lastTick = SDL_GetTicks64();
			m_pAnimation->next();
		}
	}
}

void AnimationManager::reload(size_t w, size_t h)
{
	delete m_pAnimation;
	m_pAnimation = new Animation(w, h);
}
