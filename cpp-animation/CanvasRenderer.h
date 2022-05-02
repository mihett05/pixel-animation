#pragma once

#include <SDL.h>
#include <iostream>
#include <vector>

#include "imgui.h"

#include "Canvas.h"
#include "Vector.h"
#include "Settings.h"
#include "Saver.h"
#include "constants.h"

using namespace std;

void pushAnimation(Settings* settings);

class CanvasRenderer
{
private:
	SDL_Renderer* m_pRenderer;
	vector<Canvas*> m_frames;
	Canvas* m_pBackgroundCanvas;
	size_t m_width = 32;
	size_t m_height = 32;
	Settings* m_pSettings;

	size_t m_winWidth = 0;
	size_t m_winHeight = 0;
	bool m_isMousePressed = false;

	int m_offsetX = 0;
	int m_offsetY = 0;

	Uint64 lastTick = 0;

	Vector<double> getSizeOfBlock();

	double getOriginalCoeff();
	double getCoeff();
	Vector<double> getCanvasCoords(int x, int y);
	Vector<int> getOffset();
	bool keyWasPressed(SDL_Event& e, SDL_Scancode key);

	Canvas* canvas(); // to get current canvas
	void createBackground();

	void renderBackground(SDL_Rect& rect);
	void renderCanvas(Canvas* c, bool checkPrev = false);
	void renderGrid();
	void renderMinimap();
	void renderPrevious();
	void render();

public:
	CanvasRenderer(Settings* settings, SDL_Renderer* renderer, size_t w, size_t h);
	int m_currentFrame = 0;

	void recreate(int w, int h);
	void newFrame();

	void updateWindowSize(int w, int h);
	void update(SDL_Event& e);

	void switchAnimation();
	void save(Saver* saver);

	size_t getFramesCount();
	Vector<int> getSizeInWindow();
};

