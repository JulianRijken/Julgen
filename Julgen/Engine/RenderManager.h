#pragma once
#include <SDL.h>
#include <unordered_set>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include "Singleton.h"


namespace jul
{
class RenderComponent;
	class Texture2D;


	class RenderManager final : public Singleton<RenderManager>
	{
        friend class RenderComponent;

	public:

		void Initialize(SDL_Window* window);
		void Destroy();


		void Render() const;

        void RenderTexture(const Texture2D& texture, float x, float y) const;
        void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
        void RenderTexture(const Texture2D& texture,
                           const glm::vec2 drawLocation,
                           const glm::vec2 srcLocation,
                           const glm::ivec2 cellSize,
                           int pixelsPerUnit,
                           glm::vec2 pivot,
                           bool flipX,
                           bool flipY) const;

        [[nodiscard]] SDL_Renderer* GetSDLRenderer() const { return m_RendererPtr; };
        [[nodiscard]] const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }

		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

	private:

		void RenderObjects() const;

        SDL_Renderer* m_RendererPtr{};
        SDL_Window* m_WindowPtr{};
		SDL_Color m_ClearColor{};

        // TODO: Should be in the camera component
        float m_OrthoSize = 16;

        inline static std::unordered_set<RenderComponent*> s_GlobalRendererPtrs{};
        glm::mat4 vec3(float, float, float) const;
    };
}

