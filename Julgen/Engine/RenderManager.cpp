#include "RenderManager.h"

#include <stdexcept>
#include <cstring>
#include <algorithm>
#include <vector>

#include "GUI.h"
#include "GlobalSettings.h"
#include "Renderer.h"
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
    m_WindowPtr = window;
    m_RendererPtr = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (m_RendererPtr == nullptr)
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());

    SDL_RenderSetScale(m_RendererPtr, 5.0f, 5.0f); // Reset scaling

    SetRenderOrthographic(m_OrthoSize);
}


void jul::RenderManager::Destroy()
{
    if (m_RendererPtr == nullptr)
        return;

    SDL_DestroyRenderer(m_RendererPtr);
    m_RendererPtr = nullptr;
}


void jul::RenderManager::SetRenderOrthographic([[maybe_unused]] float orthoSize)
{
    orthoSize = 40;

    SDL_Rect viewport;
    viewport.w = static_cast<int>(GlobalSettings::WINDOW_WIDTH);
    viewport.h = static_cast<int>(GlobalSettings::WINDOW_HEIGHT);
    SDL_RenderSetViewport(m_RendererPtr, &viewport);

    SDL_RenderSetLogicalSize(m_RendererPtr, GlobalSettings::RENDER_WIDTH, GlobalSettings::RENDER_HEIGHT);
}


void jul::RenderManager::Render() const
{
	const SDL_Color& color = GetBackgroundColor();
    SDL_SetRenderDrawColor(m_RendererPtr, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_RendererPtr);
	GUI::GetInstance().NewFrame();


    SDL_SetRenderDrawColor(m_RendererPtr, 255, 255, 255, 255);
    for (int i = -5; i <= 5; ++i)
    {
        for (int j = -5; j <= 5; ++j)
        {
            SDL_Rect rect{i * 50,j * 50,50,50};
            SDL_RenderDrawRect(m_RendererPtr, &rect);
        }
    }


	RenderObjects();

	GUI::GetInstance().EndFrame();
    SDL_RenderPresent(m_RendererPtr);
}


void jul::RenderManager::RenderObjects() const
{
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

	for (Renderer* renderer : renderers)
		renderer->UpdateGUI();
}




void jul::RenderManager::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
    SDL_Rect dst{};
    dst.x = static_cast<int>(x);
    dst.y = static_cast<int>(y);
    SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(m_RendererPtr, texture.GetSDLTexture(), nullptr, &dst);
}

void jul::RenderManager::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
    SDL_Rect dst{};
    dst.x = static_cast<int>(x);
    dst.y = static_cast<int>(y);
    dst.w = static_cast<int>(width);
    dst.h = static_cast<int>(height);
    SDL_RenderCopy(m_RendererPtr, texture.GetSDLTexture(), nullptr, &dst);
}

void jul::RenderManager::RenderTexture(const Texture2D& texture, const glm::vec2 drawLocation, [[maybe_unused]] const glm::vec2 srcLocation,  const glm::ivec2 cellSize) const
{
    SDL_Rect dstRect{};
    dstRect.x = static_cast<int>(drawLocation.x);
    dstRect.y = static_cast<int>(drawLocation.y);
    dstRect.w = static_cast<int>(cellSize.x);
    dstRect.h = static_cast<int>(cellSize.y);

    SDL_Rect srcRect{};
    srcRect.x = static_cast<int>(srcLocation.x);
    srcRect.y = static_cast<int>(srcLocation.y);
    srcRect.w = static_cast<int>(cellSize.x);
    srcRect.h = static_cast<int>(cellSize.y);
    SDL_RenderCopy(m_RendererPtr, texture.GetSDLTexture(), &srcRect, &dstRect);
}
