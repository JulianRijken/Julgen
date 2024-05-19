#include <stdexcept>
#include "Font.h"
#include <SDL_ttf.h>

TTF_Font* jul::Font::GetFont() const
{
	return m_Font;
}

jul::Font::Font(const std::string& fullPath, int size) :
    m_Size(size),
    m_Font(TTF_OpenFont(fullPath.c_str(), size))
{
	if (m_Font == nullptr) 
		throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
}

int jul::Font::GetSize() const { return m_Size; }

jul::Font::~Font()
{
	TTF_CloseFont(m_Font);
}
