#include "RenderManager.h"

#include <stdexcept>
#include <cstring>
#include <algorithm>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GUI.h"
#include "GlobalSettings.h"
#include "RenderComponent.h"
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

}


void jul::RenderManager::Destroy()
{
    if (m_RendererPtr == nullptr)
        return;

    SDL_DestroyRenderer(m_RendererPtr);
    m_RendererPtr = nullptr;
}

glm::vec3 WorldToScreen(const glm::vec3& worldPos, float orthoSize)
{

    // Assuming you have a camera at the origin looking down the negative z-axis
    glm::mat4 projectionMatrix = glm::ortho(-orthoSize * jul::GlobalSettings::ASPECT_RATIO, orthoSize * jul::GlobalSettings::ASPECT_RATIO, -orthoSize, orthoSize, -1.0f, 1.0f);


    glm::mat4 viewMatrix = glm::mat4(1.0f);

    glm::vec4 clipSpacePos = projectionMatrix * viewMatrix * glm::vec4(worldPos, 1.0f);

    // Perspective division
    clipSpacePos /= clipSpacePos.w;

    // Convert to clip screen space
    float x_screen = (clipSpacePos.x + 1.0f) * 0.5f * jul::GlobalSettings::WINDOW_WIDTH;
    float y_screen = (1.0f - clipSpacePos.y) * 0.5f * jul::GlobalSettings::WINDOW_HEIGHT;

    return glm::vec3(x_screen, y_screen, clipSpacePos.z);
}



void jul::RenderManager::Render() const
{
	const SDL_Color& color = GetBackgroundColor();
    SDL_SetRenderDrawColor(m_RendererPtr, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_RendererPtr);
	GUI::GetInstance().NewFrame();


    SDL_SetRenderDrawColor(m_RendererPtr, 255, 255, 255, 255);

    SDL_RenderDrawLine(m_RendererPtr,0,0,GlobalSettings::WINDOW_WIDTH,GlobalSettings::WINDOW_HEIGHT);
    SDL_RenderDrawLine(m_RendererPtr,0,GlobalSettings::WINDOW_HEIGHT,GlobalSettings::WINDOW_WIDTH,0);


	RenderObjects();

	GUI::GetInstance().EndFrame();
    SDL_RenderPresent(m_RendererPtr);
}


void jul::RenderManager::RenderObjects() const
{
	auto renderers = std::vector(s_GlobalRendererPtrs.begin(), s_GlobalRendererPtrs.end());

    const auto compareDistance = [](const RenderComponent* a, const RenderComponent* b)
		{
			return a->Transform().WorldPosition().z > b->Transform().WorldPosition().z;
		};

    const auto compareLayer = [](const RenderComponent* a, const RenderComponent* b)
		{
			return a->GetRenderLayer() < b->GetRenderLayer();
		};

	std::ranges::sort(renderers, compareDistance);
	std::ranges::stable_sort(renderers, compareLayer);

    for (const RenderComponent* renderer : renderers)
		renderer->Render();

    for (RenderComponent* renderer : renderers)
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

void jul::RenderManager::RenderTexture(const Texture2D& texture,[[maybe_unused]] const glm::vec2 drawLocation, [[maybe_unused]]const glm::vec2 srcLocation, [[maybe_unused]] const glm::ivec2 cellSize) const
{

    glm::vec3 topLeft = WorldToScreen(glm::vec3(drawLocation, 0.0f), m_OrthoSize);

    glm::vec2 cellSizeWorld = {cellSize.x / 8.0f, -(cellSize.y / 8.0f)};

    glm::vec3 bottomRight = WorldToScreen(glm::vec3(cellSizeWorld + drawLocation, 0.0f), m_OrthoSize) - topLeft;


    SDL_Rect dstRect{};
    dstRect.x = static_cast<int>(topLeft.x);
    dstRect.y = static_cast<int>(topLeft.y);
    dstRect.w = static_cast<int>(bottomRight.x);
    dstRect.h = static_cast<int>(bottomRight.y);

    SDL_Rect srcRect{};
    srcRect.x = static_cast<int>(srcLocation.x);
    srcRect.y = static_cast<int>(srcLocation.y);
    srcRect.w = static_cast<int>(cellSize.x);
    srcRect.h = static_cast<int>(cellSize.y);
    SDL_RenderCopy(m_RendererPtr, texture.GetSDLTexture(), &srcRect, &dstRect);
}
