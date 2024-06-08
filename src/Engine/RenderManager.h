#pragma once
#include <SDL.h>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <unordered_set>

#include "Camera.h"
#include "Singleton.h"


namespace jul
{
    class RenderComponent;
    class Texture2D;

    class RenderManager final : public Singleton<RenderManager>
    {
        // TOOD: Required for adding them to the set
        // should ultimatly be adressed and removed when adding propper rendering
        friend class RenderComponent;
        friend class Camera;

    public:
        void Initialize(SDL_Window* window);
        void Destroy();


        void Render() const;
        void PickCamera();

        void RenderTexture(const Texture2D& texture, float x, float y) const;
        void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
        void RenderTexture(const Texture2D& texture, const glm::vec2 drawLocation, const glm::vec2 srcLocation,
                           const glm::ivec2 cellSize, int pixelsPerUnit, glm::vec2 pivot, bool flipX = false,
                           bool flipY = false) const;

        [[nodiscard]] SDL_Renderer* GetSDLRenderer() const { return m_RendererPtr; };
        [[nodiscard]] const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }

		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

	private:
        [[nodiscard]] glm::vec3 WorldToScreen(const glm::vec3& worldPos) const;

        void RenderObjects() const;

        SDL_Renderer* m_RendererPtr{};
        SDL_Window* m_WindowPtr{};
		SDL_Color m_ClearColor{};

        Camera* m_ActiveCamera{ nullptr };


        inline static std::unordered_set<RenderComponent*> g_RendererPtrs{};
        inline static std::unordered_set<Camera*> g_Cameras{};
    };
}

