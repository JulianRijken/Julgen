#pragma once

#include "Component.h"

namespace jul
{
	class Renderer : public Component
	{

		friend class RenderManager;

	public:
		Renderer(int renderLayer = 0, bool visible = true, const std::string& name = "Renderer");
		~Renderer() override;

		Renderer(Renderer&&) = delete;
		Renderer(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		[[nodiscard]] int GetRenderLayer() const { return m_RenderLayer; }
		void SetRenderLayer(int renderLayer) { m_RenderLayer = renderLayer; }

		// TODO: Visible can just be replaced with with enabled of the component
		void SetVisible(bool visible) { m_Visible = visible; }
		[[nodiscard]] bool IsVisible() const { return m_Visible; }

	protected:

		virtual void Render() const {}

	private:

		bool m_Visible;
		int m_RenderLayer{};
	};
}

