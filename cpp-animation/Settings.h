#pragma once
#include <SDL.h>

struct Settings
{
public:
	int zoom = 1;
	int maxZoom = 8;
	float color[3] = {0, 0, 0};

	bool previousMap = true;
	bool previousAlpha = true;
	int alpha = 32;
	bool isAnimating = false;
	int delay = 1;
	SDL_TimerID lastTimerId = 0;
};

