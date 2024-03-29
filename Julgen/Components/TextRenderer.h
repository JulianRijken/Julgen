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

        TextRenderer(GameObject* parent,const std::string& text, Font* font, int renderLayer = 0, bool useAllUpper = false);

		void SetText(const std::string& text); 
		void SetColor(const SDL_Color& color);
        bool m_UseAllCaps{false}; // NOLINT - C.131: Avoid trivial getters and setters

	private:

		void Render() const override;
		void UpdateText();

        SDL_Color m_TextColor{255,255,255,255};
        SDL_Color m_LastDrawnColor{255,255,255,255};
        std::string m_Text{};
        std::string m_LastDrawnText{};
        Font* m_FontSPtr{nullptr};
        std::unique_ptr<Texture2D> m_TextTextureSPtr{};
	};
}

