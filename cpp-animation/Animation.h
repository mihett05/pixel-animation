#pragma once

#include <SDL.h>
#include <iostream>
#include <vector>

#include "imgui.h"

#include "Canvas.h"
#include "Vector.h"
#include "Settings.h"
#include "constants.h"
#include "Saver.h"


class Animation
{
private:
	vector<Canvas*> m_frames;
	int m_currentFrame = 0;
public:
	Animation(size_t w, size_t h);
	~Animation();

	Canvas* getCanvas();
	Canvas* getFrame(size_t index);
	size_t getFramesCount();

	int getCurrentFrame();
	void setCurrentFrame(int index);

	void actOnCanvas(size_t x, size_t y, SDL_Color& color);

	void newFrame();
	void deleteFrame();

	void next();
	void prev();

	void save(string fileName, Saver* saver);
	void load(string fileName, Saver* saver);
};

