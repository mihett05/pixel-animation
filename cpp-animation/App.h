#pragma once

#include <SDL.h>
#include <iostream>

#include "Canvas.h"
#include "Vector.h"
#include "CanvasRenderer.h"
#include "CanvasManager.h"
#include "Interface.h"
#include "Settings.h"
#include "constants.h"

using namespace std;


class App
{
private:
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	Settings* m_pSettings;
	CanvasManager* m_pManager;
	Interface* m_pInterface;

public:
	App();
	void render();
	void update(SDL_Event& e);
	void run();
	void destroy();
};

