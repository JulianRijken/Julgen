#include "TextRenderer.h"

#include <algorithm>
#include <stdexcept>

#include "Font.h"
#include "RenderManager.h"
#include "SDL_ttf.h"
#include "Texture2D.h"
#include "Transform.h"



jul::TextRenderer::TextRenderer(GameObject* parent,const std::string& text, Font* font, int renderLayer, bool useAllUpper) :
      RenderComponent(parent, "TextRenderer", renderLayer),
      m_UseAllCaps(useAllUpper),
      m_Text(text),
      m_FontSPtr(font)
{
	UpdateText();
}


void jul::TextRenderer::SetText(const std::string& text)
{
	if (text == m_LastDrawnText)
		return;

	m_Text = text;
	UpdateText();
}

void jul::TextRenderer::SetColor(const SDL_Color& color)
{
	m_TextColor = color;

	if (   m_TextColor.r != m_LastDrawnColor.r
		or m_TextColor.g != m_LastDrawnColor.g
		or m_TextColor.b != m_LastDrawnColor.b
		or m_TextColor.a != m_LastDrawnColor.a)
	{
		UpdateText();
    }
}


void jul::TextRenderer::Render() const
{
	if (m_TextTextureSPtr != nullptr)
    {
        const glm::vec3& pos = GetTransform().WorldPosition();
        RenderManager::GetInstance().RenderTexture(*m_TextTextureSPtr, pos.x, pos.y);
	}
}

void jul::TextRenderer::UpdateText()
{
	if (m_Text.empty())
	{
		m_TextTextureSPtr = nullptr;
		return;
	}

    std::string text = m_Text;
	if (m_UseAllCaps)
	{
        std::ranges::transform(text.begin(), text.end(), text.begin(),
                               [](const char character)
                               {
                                   // MSVC doesn't like std::toupper without static_cast
                                   // Womp womp :(
                                   return static_cast<char>(std::toupper(character));
                               });
    }

    auto* const surf = TTF_RenderText_Blended(m_FontSPtr->GetFont(), text.c_str(), m_TextColor);
    if(surf == nullptr)
        throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());


    auto* texture = SDL_CreateTextureFromSurface(RenderManager::GetInstance().GetSDLRenderer(), surf);
    if (texture == nullptr)
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());


    SDL_FreeSurface(surf);
    m_TextTextureSPtr = std::make_unique<Texture2D>(texture);


	m_LastDrawnText = m_Text;
	m_LastDrawnColor = m_TextColor;
};
