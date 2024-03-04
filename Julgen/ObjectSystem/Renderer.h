#pragma once

#include "Component.h"

namespace jul
{
	class Renderer : public Component
	{
		friend class RenderManager;

	public:
		Renderer(GameObject* parent, const std::string& name = "Renderer", int renderLayer = 0);
		~Renderer() override;

		Renderer(Renderer&&) = delete;
		Renderer(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		[[nodiscard]] int GetRenderLayer() const { return m_RenderLayer; }
		void SetRenderLayer(int renderLayer) { m_RenderLayer = renderLayer; }


	protected:

		virtual void Render() const {}

	private:

		bool m_Visible;
		int m_RenderLayer{};
	};
}

