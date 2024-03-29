#pragma once
#include <SDL_ttf.h>
#include <string>


namespace jul
{
	/**
	 * Simple RAII wrapper for a _TTF_Font
	 */
	class Font final
	{
	public:
        [[nodiscard]] TTF_Font* GetFont() const;
		explicit Font(const std::string& fullPath, unsigned int size);
		~Font();

		Font(Font&&) = delete;
		Font(const Font &) = delete;
		Font& operator= (Font&&) = delete;
		Font& operator= (const Font&) = delete;

	private:
        TTF_Font* m_Font;
	};
}
