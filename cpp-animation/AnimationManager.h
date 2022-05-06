#pragma once

#include <SDL.h>
#include <iostream>
#include <vector>

#include "imgui.h"

#include "Canvas.h"
#include "Vector.h"
#include "Settings.h"
#include "constants.h"
#include "Animation.h"

void pushAnimation(Settings* settings);

class AnimationManager
{
private:
	Settings* m_pSettings;
	Animation* m_pAnimation;

	Uint64 lastTick = 0;
public:
	AnimationManager(Settings* settings);

	Animation* getAnimation();

	SDL_Color getCurrentColor();

	void update(SDL_Event& e);
	void reload(size_t w, size_t h);
};

