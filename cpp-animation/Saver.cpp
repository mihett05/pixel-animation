#include "Saver.h"

Saver::Saver()
{
}

void Saver::save(string fileName, vector<Canvas*>& frames, int frameWidth, int frameHeight)
{
	SDL_Surface* resultSurface = createEmptySurface(frameWidth * frames.size(), frameHeight);
	for (size_t i = 0; i < frames.size(); ++i)
	{
		SDL_Rect dest = {
			frameWidth * i, 0,
			frameWidth, frameHeight
		};
		SDL_BlitSurface(frames[i]->m_pSurface, nullptr, resultSurface, &dest);
	}
	IMG_SavePNG(resultSurface, fileName.c_str());
	SDL_FreeSurface(resultSurface);
}

SDL_Surface* Saver::load(string fileName)
{
	return IMG_Load(fileName.c_str());
}
