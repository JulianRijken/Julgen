#pragma once
#include <string>

struct _TTF_Font;

namespace jul
{
	/**
	 * Simple RAII wrapper for a _TTF_Font
	 */
	class Font final
	{
	public:
		_TTF_Font* GetFont() const;
		explicit Font(const std::string& fullPath, unsigned int size);
		~Font();

		Font(Font&&) = delete;
		Font(const Font &) = delete;
		Font& operator= (Font&&) = delete;
		Font& operator= (const Font&) = delete;

	private:
		_TTF_Font* m_Font;
	};
}
