#pragma once
#include <memory>
#include <string>
#include "RenderComponent.h"
#include "SDL_pixels.h"

namespace jul
{
	class Texture2D;

	class Font;
	class Texture;

	class TextRenderer final : public RenderComponent
	{
	public:

		TextRenderer(const std::string& text, std::shared_ptr<Font> font, int renderLayer = 0, bool visible = true, const std::string& name = "TextRenderer");
		TextRenderer(const std::string& text, std::shared_ptr<Font> font,SDL_Color m_TextColor, int renderLayer = 0, bool visible = true, const std::string& name = "TextRenderer");

		void SetText(const std::string& text);
		void Render() const override;

	private:

		void UpdateText();

		std::string m_Text;
		std::string m_LastDrawnText;
		SDL_Color m_TextColor;
		std::shared_ptr<Font> m_FontSPtr;
		std::shared_ptr<Texture2D> m_TextTextureSPtr;

	};
}

