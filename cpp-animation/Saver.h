#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

#include "Canvas.h"


class Saver
{
public:
	Saver();

	void save(vector<Canvas*>& frames, int frameWidth, int frameHeight);
};

