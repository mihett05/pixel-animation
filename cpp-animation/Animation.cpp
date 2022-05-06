#include "Animation.h"

Animation::Animation(size_t w, size_t h)
{
    m_frames.push_back(new Canvas(w, h));
    m_currentFrame = 0;
}

Animation::~Animation()
{
	m_frames.clear();
}

Canvas* Animation::getCanvas()
{
    return m_frames[m_currentFrame];
}

Canvas* Animation::getFrame(size_t index)
{
	assert(index >= 0 && index < getFramesCount());

	return m_frames[index];
}

size_t Animation::getFramesCount()
{
	return m_frames.size();
}

int Animation::getCurrentFrame()
{
	return m_currentFrame;
}

void Animation::setCurrentFrame(int index)
{
	assert(index >= 0 && index <= getFramesCount());
	m_currentFrame = index;
}

void Animation::actOnCanvas(size_t x, size_t y, SDL_Color& color)
{
	getCanvas()->setPixel(x, y, color);
}

void Animation::newFrame()
{
    m_frames.push_back(new Canvas(m_frames[0]->getWidth(), m_frames[0]->getHeight()));
    ++m_currentFrame;
}

void Animation::deleteFrame()
{
	if (getFramesCount() - 1 > 0)
	{
		int frameToDelete = m_currentFrame;
		delete m_frames[frameToDelete];
		m_frames.erase(m_frames.begin() + frameToDelete);
		if (m_currentFrame != 0)
			--m_currentFrame;
	}
}

void Animation::next()
{
	m_currentFrame = (m_currentFrame + 1) % getFramesCount();
}

void Animation::prev()
{
	if (m_currentFrame - 1 >= 0)
		--m_currentFrame;
	else
		m_currentFrame = getFramesCount() - 1;
}

void Animation::save(string fileName, Saver* saver)
{
	saver->save(fileName, m_frames, m_frames[0]->getWidth(), m_frames[0]->getHeight());
}

void Animation::load(string fileName, Saver* saver)
{
	saver->load(fileName, m_frames);
}
