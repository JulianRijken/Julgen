#include "TextRenderer.h"

#include <algorithm>
#include <stdexcept>

#include "Font.h"
#include "RenderManager.h"
#include "SDL_ttf.h"
#include "Texture2D.h"
#include "Transform.h"

jul::TextRenderer::TextRenderer(GameObject* parent, std::string text, Font* font, int renderLayer, glm::vec2 alighnment,
                                bool useAllUpper, double lineSpacing, SDL_Color textColor) :
    RenderComponent(parent, "TextRenderer", renderLayer),
    m_UseAllCaps(useAllUpper),
    m_LineSpacing(lineSpacing),
    m_TextColor(textColor),
    m_Text(std::move(text)),
    m_FontSPtr(font),
    m_Alighnment(alighnment)
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
    if(m_TextTextureUPtr != nullptr)
    {
        const glm::vec2& pos = GetTransform().GetWorldPosition();
        auto textureSize = m_TextTextureUPtr->GetSize();


        RenderManager::GetInstance().RenderTexture(
            *m_TextTextureUPtr, pos, {}, { textureSize.x, textureSize.y }, m_FontSPtr->GetSize(), m_Alighnment);
    }
}

void jul::TextRenderer::UpdateText()
{
    if(m_Text.empty())
    {
        m_TextTextureUPtr = nullptr;
        return;
    }

    std::string text = m_Text;
    if(m_UseAllCaps)
    {
        std::ranges::transform(text.begin(),
                               text.end(),
                               text.begin(),
                               [](const char character)
                               {
                                   // MSVC doesn't like std::toupper without static_cast
                                   // Womp womp :(
                                   return static_cast<char>(std::toupper(character));
                               });
    }

    // Split text into lines
    std::vector<std::string> lines;
    size_t start = 0;
    size_t end = text.find('\n');
    while(end != std::string::npos)
    {
        lines.push_back(text.substr(start, end - start));
        start = end + 1;
        end = text.find('\n', start);
    }
    lines.push_back(text.substr(start));

    // Variables to hold the total width and height of the final texture
    int totalWidth = 0;
    int totalHeight = 0;
    std::vector<SDL_Surface*> surfaces;

    // Render each line
    for(const auto& line : lines)
    {
        auto* const surf = TTF_RenderText_Blended(m_FontSPtr->GetFont(), line.c_str(), m_TextColor);
        if(surf == nullptr)
            throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

        surfaces.push_back(surf);

        if(surf->w > totalWidth)
            totalWidth = surf->w;

        totalHeight += surf->h;
    }

    totalHeight +=
        std::max(0, static_cast<int>(m_LineSpacing * m_FontSPtr->GetSize()) * (static_cast<int>(lines.size()) - 1));

    // Create a surface for the final texture
    SDL_Surface* finalSurface =
        SDL_CreateRGBSurface(0, totalWidth, totalHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if(finalSurface == nullptr)
    {
        for(auto* surf : surfaces)
            SDL_FreeSurface(surf);

        throw std::runtime_error(std::string("Create RGB surface failed: ") + SDL_GetError());
    }

    // Copy each line surface onto the final surface
    int currentY = 0;
    for(auto* surf : surfaces)
    {
        // Allows text to center even new lines
        const int xOffset = static_cast<int>((totalWidth - surf->w) * m_Alighnment.x);

        SDL_Rect dstRect = { xOffset, currentY, surf->w, surf->h };
        SDL_BlitSurface(surf, nullptr, finalSurface, &dstRect);
        currentY += surf->h + static_cast<int>(m_LineSpacing * m_FontSPtr->GetSize());
        SDL_FreeSurface(surf);
    }

    // Create the final texture from the final surface
    auto* texture = SDL_CreateTextureFromSurface(RenderManager::GetInstance().GetSDLRenderer(), finalSurface);
    SDL_FreeSurface(finalSurface);

    if(texture == nullptr)
        throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

    m_TextTextureUPtr = std::make_unique<Texture2D>(texture);
    m_LastDrawnText = m_Text;
    m_LastDrawnColor = m_TextColor;
};
