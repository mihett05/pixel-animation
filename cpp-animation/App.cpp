#include "App.h"


App::App()
{
	m_pWindow = SDL_CreateWindow("Animation", 100, 100, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	m_pSettings = new Settings();

	int w, h;
	SDL_GetWindowSize(m_pWindow, &w, &h);

	m_pCanvasRenderer = new CanvasRenderer(m_pSettings, m_pRenderer, w, h);
	m_pInterface = new Interface(m_pSettings, m_pCanvasRenderer);
	m_pInterface->init(m_pWindow, m_pRenderer, w, h);
}

void App::render()
{
}

void App::update(SDL_Event& e)
{
	// update texture size on window resize
	int w, h;
	SDL_GetWindowSize(m_pWindow, &w, &h);

	// because of menu bar
	SDL_Rect viewport = {
		0, menuBarOffset,
		w, h - menuBarOffset,
	};
	SDL_RenderSetViewport(m_pRenderer, &viewport);

	m_pCanvasRenderer->updateWindowSize(w, h - menuBarOffset);

	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 0xff);
	SDL_RenderClear(m_pRenderer);
	
	
	m_pCanvasRenderer->update(e);
	m_pInterface->update(e);
	
	SDL_RenderPresent(m_pRenderer);
}

void App::run()
{
	bool quit = false;
	bool isMousePressed = false;
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{	
			ImGui_ImplSDL2_ProcessEvent(&e);
			if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) quit = true;
			update(e);
		}
		//SDL_Delay(10);
	}
	destroy();
}

void App::destroy()
{
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}
