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

bool Saver::load(string fileName, vector<Canvas*>& frames)
{
	SDL_Surface* sprites = IMG_Load(fileName.c_str());
	
	if (sprites != nullptr)
	{
		int count = sprites->w / sprites->h;
		int size = sprites->h;

		frames.clear();
		frames.reserve(count);

		SDL_Rect copyRect = {
			0, 0,
			size, size
		};

		for (size_t i = 0; i < count; ++i)
		{
			SDL_Surface* surface = createEmptySurface(size, size);
			SDL_BlitSurface(sprites, &copyRect, surface, nullptr);
			copyRect.x += size;
			frames.push_back(new Canvas(surface));
		}
	}
	
	return sprites != nullptr;
}
