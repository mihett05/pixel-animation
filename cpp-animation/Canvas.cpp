#include "Canvas.h"



Canvas::Canvas(size_t w, size_t h)
{
	Uint32 rmask, gmask, bmask, amask;

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif
    m_pSurface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
	
	update();
}

Canvas::~Canvas()
{
    SDL_FreeSurface(m_pSurface);
}

void Canvas::update()
{
	SDL_Rect fill = {
		0, 0,
		getWidth(), getHeight()
	};;
	SDL_FillRect(m_pSurface, &fill, SDL_MapRGBA(m_pSurface->format, 255, 255, 255, 0));
	/*unsigned int* ptr = (unsigned int*)m_pSurface->pixels;
	for (int x = 0; x < m_pSurface->w; x++) {
		for (int y = 0; y < m_pSurface->h; y++) {
			int xr = 255 * x / m_pSurface->w;
			int yr = 255 * y / m_pSurface->h;
			int row = y * (m_pSurface->pitch / m_pSurface->format->BytesPerPixel);
			// int Pixel = x * screen->format->BytesPerPixel;
			ptr[row + x] = SDL_MapRGBA(m_pSurface->format, xr, 255 - yr, 255 - xr, 255);
		}
	}*/
}


void Canvas::setPixel(size_t x, size_t y, SDL_Color color)
{
    SDL_Rect pixel = { x, y, 1, 1 };
    
    SDL_FillRect(m_pSurface, &pixel, SDL_MapRGB(m_pSurface->format, color.r, color.g, color.b));
}

void Canvas::setPixel(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b)
{
    SDL_Rect pixel = { x, y, 1, 1 };
    SDL_FillRect(m_pSurface, &pixel, SDL_MapRGB(m_pSurface->format, r, g, b));
}

size_t Canvas::getWidth()
{
	return m_pSurface->w;
}

size_t Canvas::getHeight()
{
	return m_pSurface->h;
}
