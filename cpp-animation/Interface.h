#pragma once

#include <SDL.h>
#include <iostream>
#include <string>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "ImGuiFileDialog.h"

#include "Canvas.h"
#include "Vector.h"
#include "CanvasManager.h"
#include "Settings.h"
#include "Saver.h"

using namespace std;

class Interface
{
private:
	CanvasManager* m_pManager;
	Settings* m_pSettings;
	Saver* m_pSaver;

	void renderTool();
	void renderMenu();
	void renderFrames();
	void renderAnimation();
	void render();
public:
	Interface(Settings* settings, CanvasManager* renderer);
	void init(SDL_Window* win, SDL_Renderer* renderer, int w, int h);
	void update(SDL_Event& e);
};

