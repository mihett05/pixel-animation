#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

#include "Canvas.h"

using namespace std;

class Saver
{
public:
	Saver();

	void save(string fileName, vector<Canvas*>& frames, int frameWidth, int frameHeight);
	bool load(string fileName, vector<Canvas*>& frames);
};

