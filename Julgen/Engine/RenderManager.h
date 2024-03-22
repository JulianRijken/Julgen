#pragma once
#include <SDL.h>
#include <unordered_set>
#include <glm/vec2.hpp>

#include "Singleton.h"

namespace jul
{
	class Renderer;
	class Texture2D;


	class RenderManager final : public Singleton<RenderManager>
	{
		friend class Renderer;

	public:

		void Initialize(SDL_Window* window);
		void Destroy();

        void SetRenderOrthographic(float orthoSize);

		void Render() const;

        void RenderTexture(const Texture2D& texture, float x, float y) const;
        void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
        void RenderTexture(const Texture2D& texture, const glm::vec2 drawLocation, const glm::vec2 srcLocation,  const glm::ivec2 cellSize) const;

        [[nodiscard]] SDL_Renderer* GetSDLRenderer() const { return m_RendererPtr; };
        [[nodiscard]] const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }

		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

	private:

		void RenderObjects() const;

        SDL_Renderer* m_RendererPtr{};
        SDL_Window* m_WindowPtr{};
		SDL_Color m_ClearColor{};

        // TODO: Should be in the camera component
        float m_OrthoSize = 15;

		inline static std::unordered_set<Renderer*> s_GlobalRendererPtrs{};
	};
}

