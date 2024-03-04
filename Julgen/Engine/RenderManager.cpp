#include "RenderManager.h"

#include <stdexcept>
#include <cstring>
#include <algorithm>
#include <set>

#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "Transform.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void jul::RenderManager::Initialize(SDL_Window* window)
{
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (m_Renderer == nullptr) 
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
}

void jul::RenderManager::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);


	auto renderers = std::vector(s_GlobalRendererPtrs.begin(), s_GlobalRendererPtrs.end());

	const auto compareDistance = [](const Renderer* a, const Renderer* b)
	{
		return a->Transform().WorldPosition().z > b->Transform().WorldPosition().z;
	};

	const auto compareLayer = [](const Renderer* a, const Renderer* b)
		{
			return a->GetRenderLayer() < b->GetRenderLayer();
		};

	std::ranges::sort(renderers, compareDistance);
	std::ranges::stable_sort(renderers, compareLayer);

	for (const Renderer* renderer : renderers)
		renderer->Render();


	
	SDL_RenderPresent(m_Renderer);
}

void jul::RenderManager::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void jul::RenderManager::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void jul::RenderManager::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}
