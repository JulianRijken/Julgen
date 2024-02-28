#pragma once
#include <glm/vec2.hpp>

struct SDL_Texture;

namespace jul
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D final
	{
	public:

		explicit Texture2D(SDL_Texture* texture);

		~Texture2D();

		[[nodiscard]] SDL_Texture* GetSDLTexture() const;
		[[nodiscard]] glm::ivec2 GetSize() const;

		Texture2D(Texture2D &&) = delete;
		Texture2D(const Texture2D &) = delete;
		Texture2D & operator= (Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;

	private:
		SDL_Texture* m_Texture;
	};
}
