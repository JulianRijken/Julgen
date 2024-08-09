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

#include "EngineGUI.h"
#include "GameSettings.h"
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
    m_RendererPtr =
        SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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


void jul::RenderManager::Render() const
{
    if(m_ActiveCamera == nullptr)
        return;

    const SDL_Color& color = GetBackgroundColor();
    SDL_SetRenderDrawColor(m_RendererPtr, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_RendererPtr);

    EngineGUI::NewFrame();

    // Render Objects in render size
    SDL_RenderSetLogicalSize(m_RendererPtr, GameSettings::g_RenderWidth, GameSettings::g_RenderHeight);
    RenderObjects();

    // Render GUI In window size
    SDL_RenderSetLogicalSize(m_RendererPtr, GameSettings::g_WindowWidth, GameSettings::g_WindowHeight);
    EngineGUI::EndFrame();

    SDL_RenderPresent(m_RendererPtr);
}

void jul::RenderManager::PickCamera()
{
    if(g_Cameras.empty())
    {
        m_ActiveCamera = nullptr;
    }
    else
    {
        auto cameras = std::vector(g_Cameras.begin(), g_Cameras.end());
        m_ActiveCamera = *std::ranges::max_element(
            cameras, [](const Camera* a, const Camera* b) { return a->GetPriority() > b->GetPriority(); });
    }
}

void jul::RenderManager::RenderObjects() const
{

    ////////////////////////////////////////////////////////////////////////
    /// TODO: RENDERING NEEDS OVERHULL USING SHADERS AND PROPPER METHODS ///
    /// NOT THIS HORRIBLE CPU SORTING EVERY FRAME :)                     ///
    ////////////////////////////////////////////////////////////////////////
    // Sort cameras by priority


    auto renderers = std::vector(g_RendererPtrs.begin(), g_RendererPtrs.end());

    const auto compareDistance = [](const RenderComponent* a, const RenderComponent* b)
		{
			return a->GetTransform().GetWorldPosition().z > b->GetTransform().GetWorldPosition().z;
		};

    const auto compareLayer = [](const RenderComponent* a, const RenderComponent* b)
		{
			return a->GetRenderLayer() < b->GetRenderLayer();
		};


    const auto isActive =
        std::ranges::remove_if(renderers,
                               [](const RenderComponent* rendererPtr) { return not rendererPtr->IsEnabledAndActive(); })
            .begin();

    renderers.resize(std::distance(renderers.begin(), isActive));

    std::ranges::sort(renderers, compareDistance);
    std::ranges::stable_sort(renderers, compareLayer);

    for (const RenderComponent* renderer : renderers)
		renderer->Render();

    for (RenderComponent* renderer : renderers)
		renderer->UpdateGUI();
}

void jul::RenderManager::RenderTexture(const Texture2D& texture, const glm::vec2& drawLocation, float drawAngle,
                                       glm::vec2 drawScale, const glm::vec2& srcLocation, const glm::ivec2& cellSize,
                                       int pixelsPerUnit, const glm::vec2& pivot, bool flipX, bool flipY) const
{

    const glm::vec3 topLeft = WorldToScreen(glm::vec3(drawLocation, 0.0f));
    const glm::vec2 cellSizeWorld = { static_cast<float>(cellSize.x) / static_cast<float>(pixelsPerUnit),
                                      -(static_cast<float>(cellSize.y) / static_cast<float>(pixelsPerUnit)) };
    const glm::vec3 rectSize = WorldToScreen(glm::vec3(cellSizeWorld + drawLocation, 0.0f)) - topLeft;

    const glm::vec2 scaledRectSize = glm::vec2(rectSize) * drawScale;

    SDL_Rect dstRect{};
    dstRect.x = static_cast<int>(std::round(topLeft.x - scaledRectSize.x * pivot.x));
    dstRect.y = static_cast<int>(std::round(topLeft.y - scaledRectSize.y * pivot.y));
    dstRect.w = static_cast<int>(std::round(scaledRectSize.x));
    dstRect.h = static_cast<int>(std::round(scaledRectSize.y));

    SDL_Rect srcRect{};
    srcRect.x = static_cast<int>(std::round(srcLocation.x));
    srcRect.y = static_cast<int>(std::round(srcLocation.y));
    srcRect.w = static_cast<int>(std::round(cellSize.x));
    srcRect.h = static_cast<int>(std::round(cellSize.y));

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if(flipX and flipY)
        flip = static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    else if(flipX)
        flip = SDL_FLIP_HORIZONTAL;
    else if(flipY)
        flip = SDL_FLIP_VERTICAL;


    const SDL_Point center{ static_cast<int>(std::round(static_cast<float>(cellSize.x) * pivot.x)),
                            static_cast<int>(std::round(static_cast<float>(cellSize.y) * pivot.y)) };  // Not needed
    SDL_RenderCopyEx(m_RendererPtr, texture.GetSDLTexture(), &srcRect, &dstRect, drawAngle, &center, flip);
}

// Converts from world to screen pixels
glm::vec3 jul::RenderManager::WorldToScreen(const glm::vec3& worldPos) const
{
    assert(m_ActiveCamera);

    glm::vec4 clipSpacePos = m_ActiveCamera->GetViewPorjection() * glm::vec4(worldPos, 1.0f);

    // Perspective division
    clipSpacePos /= clipSpacePos.w;

    // Convert to clip screen space
    float x_screen = (clipSpacePos.x + 1.0f) * 0.5f *
                     static_cast<float>(jul::GameSettings::g_RenderWidth);  // TODO: maybe remove 0.5f
    float y_screen = (1.0f - clipSpacePos.y) * 0.5f * static_cast<float>(jul::GameSettings::g_RenderHeight);

    return { x_screen, y_screen, clipSpacePos.z };
}
