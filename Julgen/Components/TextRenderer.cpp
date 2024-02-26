#include "TextRenderer.h"

#include <stdexcept>
#include <utility>

#include "Font.h"
#include "RenderManager.h"
#include "SDL_ttf.h"
#include "Texture2D.h"
#include "Transform.h"

jul::TextRenderer::TextRenderer(const std::string& text, std::shared_ptr<Font> font, int renderLayer, bool visible, const std::string& name):
	Renderer(renderLayer, visible, name),
	m_TextColor(255,255,255,255),
	m_Text(text),
	m_FontSPtr(std::move(font)),
	m_TextTextureSPtr(nullptr)
{
	UpdateText();
}

jul::TextRenderer::TextRenderer(const std::string& text, std::shared_ptr<Font> font, SDL_Color m_TextColor, int renderLayer, bool visible, const std::string& name) :
	Renderer(renderLayer, visible, name),
	m_TextColor(m_TextColor),
	m_Text(text),
	m_FontSPtr(std::move(font)),
	m_TextTextureSPtr(nullptr)
{
	UpdateText();
}

void jul::TextRenderer::SetText(const std::string& text)
{
	m_Text = text;
	UpdateText();
}

void jul::TextRenderer::Render() const
{
	if (m_TextTextureSPtr != nullptr)
	{
		auto& pos = Transform().Position();
		RenderManager::GetInstance().RenderTexture(*m_TextTextureSPtr, pos.x, pos.y);
	}
}

void jul::TextRenderer::UpdateText()
{
	if (m_Text != m_LastDrawnText || m_TextTextureSPtr == nullptr)
	{
		const auto surf = TTF_RenderText_Blended(m_FontSPtr->GetFont(), m_Text.c_str(), m_TextColor);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(RenderManager::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}

		SDL_FreeSurface(surf);
		m_TextTextureSPtr = std::make_unique<Texture2D>(texture);

		m_LastDrawnText = m_Text;
	}

	if (m_Text.empty())
		m_TextTextureSPtr = nullptr;
}
