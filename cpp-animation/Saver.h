#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

#include "Canvas.h"
#include "Vector.h"
#include "CanvasRenderer.h"
#include "Settings.h"

class Saver
{
public:
	Saver();

	void save(vector<Canvas*>& frames, int frameWidth, int frameHeight);
};

