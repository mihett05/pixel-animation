#pragma once

#include <SDL.h>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "Canvas.h"
#include "Vector.h"
#include "CanvasRenderer.h"
#include "Interface.h"
#include "Settings.h"

using namespace std;


class App
{
private:
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	Settings* m_pSettings;
	CanvasRenderer* m_pCanvasRenderer;
	Interface* m_pInterface;

public:
	App();
	void render();
	void update(SDL_Event& e);
	void run();
	void destroy();
};

