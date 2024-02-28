#pragma once
#include <SDL.h>
#include <unordered_set>

#include "Singleton.h"

namespace jul
{
	class Renderer;
	class Texture2D;


	class RenderManager final : public Singleton<RenderManager>
	{
		friend class Singleton;
		friend class Renderer;

	public:

		void Initialize(SDL_Window* window);
		void Destroy();

		void Render() const;

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		[[nodiscard]] SDL_Renderer* GetSDLRenderer() const { return m_Renderer; };
		[[nodiscard]] const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }

		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

	private:

		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_ClearColor{};

		inline static std::unordered_set<Renderer*> s_GlobalRendererPtrs{};
	};
}

