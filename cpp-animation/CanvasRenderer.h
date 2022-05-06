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
#include "AnimationManager.h"

using namespace std;

class CanvasRenderer
{
private:
	SDL_Renderer* m_pRenderer;
	Canvas* m_pBackgroundCanvas;
	Settings* m_pSettings;
	Animation* m_pAnimation;

	size_t m_winWidth = 0;
	size_t m_winHeight = 0;

	int m_offsetX = 0;
	int m_offsetY = 0;

	Canvas* canvas();

	void createBackground();

	void renderBackground(SDL_Rect& rect);
	void renderCanvas(Canvas* c, bool checkPrev = false);
	void renderGrid();
	void renderMinimap();
	void renderPrevious();
	void render();

public:
	CanvasRenderer(Settings* settings, SDL_Renderer* renderer, Animation* animation, size_t winWidth, size_t winHeight);

	Vector<double> getSizeOfBlock();

	double getOriginalCoeff();
	double getCoeff();
	Vector<double> getCanvasCoords(int x, int y);
	Vector<int> getOffset();

	void reload(Animation* animation);

	void renderSelected(SDL_Event& e);
	bool isInsideCanvas(int x, int y);

	void zoom(bool isZooming);
	void shift(int x, int y);

	void updateWindowSize(int w, int h);
	void update(SDL_Event& e);

	Vector<int> getSizeInWindow();
};

