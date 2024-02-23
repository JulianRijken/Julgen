#pragma once
#include "Renderer.h"

#include <memory>
#include "Texture2D.h"

namespace jul
{
	class SpriteRenderer final : public Renderer
	{
	public:

		
	private:

		SpriteRenderer(const std::string& fileName = {}, int renderLayer = 0, bool visible = true, const std::string& name = "SpriteRenderer");


		void Render() const override;

		std::shared_ptr<Texture2D> m_TextureSPtr;
	};
}

