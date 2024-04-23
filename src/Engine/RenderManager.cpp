#include "RenderManager.h"

#include <algorithm>
#include <cstring>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <stdexcept>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "GameSettings.h"
#include "GUI.h"
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

    SDL_RenderSetLogicalSize(m_RendererPtr,GameSettings::s_RenderWidth,GameSettings::s_RenderHeight);
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
    float aspectRatio = jul::GameSettings::GetAspectRatio();

    // Assuming you have a camera at the origin looking down the negative z-axis
    glm::mat4 projectionMatrix =
        glm::ortho(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, -1.0f, 1.0f);

    glm::mat4 viewMatrix = glm::mat4(1.0f); // TODO: Implement camera

    glm::vec4 clipSpacePos = projectionMatrix * viewMatrix * glm::vec4(worldPos, 1.0f);

    // Perspective division
    clipSpacePos /= clipSpacePos.w;

    // Convert to clip screen space
    float x_screen = (clipSpacePos.x + 1.0f) * 0.5f * jul::GameSettings::s_RenderWidth;  // TODO: maybe remove 0.5f
    float y_screen = (1.0f - clipSpacePos.y) * 0.5f * jul::GameSettings::s_RenderHeight;

    return glm::vec3(x_screen, y_screen, clipSpacePos.z);
}



void jul::RenderManager::Render() const
{
	const SDL_Color& color = GetBackgroundColor();
    SDL_SetRenderDrawColor(m_RendererPtr, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_RendererPtr);
    EngineGUI::NewFrame();
    {
        RenderObjects();
    }
    EngineGUI::EndFrame();
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

void jul::RenderManager::RenderTexture(const Texture2D& texture, const glm::vec2 drawLocation,
                                       const glm::vec2 srcLocation, const glm::ivec2 cellSize, int pixelsPerUnit,
                                       glm::vec2 pivot, bool flipX, bool flipY) const
{

    const glm::vec3 topLeft = WorldToScreen(glm::vec3(drawLocation, 0.0f), m_OrthoSize);
    const glm::vec2 cellSizeWorld = { static_cast<float>(cellSize.x) / static_cast<float>(pixelsPerUnit),
                                      -(static_cast<float>(cellSize.y) / static_cast<float>(pixelsPerUnit)) };
    const glm::vec3 rectSize = WorldToScreen(glm::vec3(cellSizeWorld + drawLocation, 0.0f), m_OrthoSize) - topLeft;

    SDL_Rect dstRect{};
    dstRect.x = static_cast<int>(topLeft.x - rectSize.x * pivot.x);
    dstRect.y = static_cast<int>(topLeft.y - rectSize.y * pivot.y);
    dstRect.w = static_cast<int>(rectSize.x);
    dstRect.h = static_cast<int>(rectSize.y);

    SDL_Rect srcRect{};
    srcRect.x = static_cast<int>(srcLocation.x);
    srcRect.y = static_cast<int>(srcLocation.y);
    srcRect.w = static_cast<int>(cellSize.x);
    srcRect.h = static_cast<int>(cellSize.y);

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (flipX and flipY)
        flip = static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    else if(flipX)
        flip = SDL_FLIP_HORIZONTAL;
    else if(flipY)
        flip = SDL_FLIP_VERTICAL;


    const SDL_Point center{};  // Not needed
    SDL_RenderCopyEx(m_RendererPtr, texture.GetSDLTexture(), &srcRect, &dstRect,0.0f,&center,flip);
}
