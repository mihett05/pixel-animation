#pragma once

#include <SDL.h>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "Canvas.h"
#include "Vector.h"
#include "Settings.h"

using namespace std;

class CanvasRenderer
{
private:
	SDL_Renderer* m_pRenderer;
	Canvas* m_pCanvas;
	Settings* m_pSettings;

	size_t m_winWidth = 0;
	size_t m_winHeight = 0;
	bool m_isMousePressed = false;

	int m_offsetX = 0;
	int m_offsetY = 0;

	Vector<double> getSizeOfBlock();

	double getOriginalCoeff();
	double getCoeff();
	Vector<double> getCanvasCoords(int x, int y);
	Vector<int> getOffset();
	bool keyWasPressed(SDL_Event& e, SDL_Scancode key);

	Canvas* canvas(); // to get current canvas

	void renderCanvas();
	void renderGrid();
	void renderMinimap();
	void render();

public:
	CanvasRenderer(Settings* settings, SDL_Renderer* renderer, size_t w, size_t h);

	void recreate(int w, int h);
	void updateWindowSize(int w, int h);
	void update(SDL_Event& e);
};

