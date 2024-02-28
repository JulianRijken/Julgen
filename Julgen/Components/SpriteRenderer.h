#pragma once
#include "Renderer.h"

namespace jul
{
	class Sprite;

	class SpriteRenderer final : public Renderer
	{
	public:
		SpriteRenderer(Sprite* sprite = nullptr, int renderLayer = 0, bool visible = true);

	private:

		void Render() const override;

		Sprite* m_SpritePtr;
	};
}

