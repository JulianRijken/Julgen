#pragma once
#include <memory>
#include <string>

#include "Renderer.h"
#include "SDL_pixels.h"

namespace jul
{
	class Texture2D;

	class Font;
	class Texture;

	class TextRenderer final : public Renderer
	{
	public:

		TextRenderer(const std::string& text, std::shared_ptr<Font> font, int renderLayer = 0, bool visible = true, const std::string& name = "TextRenderer");
		TextRenderer(const std::string& text, std::shared_ptr<Font> font,SDL_Color m_TextColor, int renderLayer = 0, bool visible = true, const std::string& name = "TextRenderer");

		void SetText(const std::string& text);

	private:

		void Render() const override;
		void UpdateText();

		SDL_Color m_TextColor;
		std::string m_Text;
		std::string m_LastDrawnText;
		std::shared_ptr<Font> m_FontSPtr;
		std::unique_ptr<Texture2D> m_TextTextureSPtr;

	};
}

