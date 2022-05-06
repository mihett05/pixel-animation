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
#include "CanvasRenderer.h"
#include "AnimationManager.h"

class CanvasManager
{
private:
	Settings* m_pSettings;
	CanvasRenderer* m_pRenderer;
	AnimationManager* m_pAnimationManager;

	bool m_isMousePressed = false;

	bool keyWasPressed(SDL_Event& e, SDL_Scancode key);

public:
	CanvasManager(Settings* settings);

	CanvasRenderer* getRenderer();
	AnimationManager* getAnimationManager();
	Animation* getAnimation();

	void createRenderer(SDL_Renderer* renderer, size_t winWidth, size_t winHeight);

	void reload(int w, int h);

	void updateWindowSize(int w, int h);
	void update(SDL_Event& e);
};

