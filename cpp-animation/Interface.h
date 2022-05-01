#pragma once

#include <SDL.h>
#include <iostream>
#include <string>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "Canvas.h"
#include "Vector.h"
#include "CanvasRenderer.h"
#include "Settings.h"

using namespace std;

class Interface
{
private:
	CanvasRenderer* m_pRenderer;
	Settings* m_pSettings;

	void renderBrush();
	void renderMenu();
	void render();
public:
	Interface(Settings* settings, CanvasRenderer* renderer);
	void init(SDL_Window* win, SDL_Renderer* renderer, int w, int h);
	void update(SDL_Event& e);
};
