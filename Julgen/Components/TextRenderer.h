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

		TextRenderer(GameObject* parent, const std::string& text, Font* font, int renderLayer = 0);
		TextRenderer(GameObject* parent, const std::string& text, Font* font,SDL_Color m_TextColor, int renderLayer = 0);

		void SetText(const std::string& text); 
		void SetColor(const SDL_Color& color);

	private:

		void Render() const override;
		void UpdateText();

		SDL_Color m_TextColor;
		SDL_Color m_LastDrawnColor;
		std::string m_Text;
		std::string m_LastDrawnText;
		Font* m_FontSPtr;
		std::unique_ptr<Texture2D> m_TextTextureSPtr;
	};
}

