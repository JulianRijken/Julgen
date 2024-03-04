#pragma once
#include "Renderer.h"

namespace jul
{
	class Sprite;

	class SpriteRenderer final : public Renderer
	{
	public:
		SpriteRenderer(GameObject* parent, Sprite* sprite = nullptr, int renderLayer = 0);

	private:

		void Render() const override;

		Sprite* m_SpritePtr;
	};
}

