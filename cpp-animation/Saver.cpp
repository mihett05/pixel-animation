#include "Saver.h"

Saver::Saver()
{
}

void Saver::save(vector<Canvas*>& frames, int frameWidth, int frameHeight)
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

	SDL_Surface* resultSurface = SDL_CreateRGBSurface(0, frameWidth * frames.size(), frameHeight, 32, rmask, gmask, bmask, amask);
	for (size_t i = 0; i < frames.size(); ++i)
	{
		SDL_Rect dest = {
			frameWidth * i, 0,
			frameWidth, frameHeight
		};
		SDL_BlitSurface(frames[i]->m_pSurface, nullptr, resultSurface, &dest);
	}
	IMG_SavePNG(resultSurface, "test.png");
	SDL_FreeSurface(resultSurface);
}
