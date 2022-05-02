#pragma once
#include <SDL.h>
#include <cstdint>

using namespace std;


class Canvas // CanvASS
{
public:
	SDL_Surface* m_pSurface;

	Canvas(size_t w, size_t h);
	~Canvas();

	void update();

	void setPixel(size_t x, size_t y, SDL_Color color);
	void setPixel(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b);
	void setRect(SDL_Rect& rect, SDL_Color color);

	size_t getWidth();
	size_t getHeight();
};

