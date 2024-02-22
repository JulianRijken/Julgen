#pragma once
#include "Component.h"

namespace jul
{
	class Renderer : public Component
	{

	public:
		Renderer(int renderLayer = 0, bool visible = true, const std::string& name = "Renderer");
		~Renderer() override = default;

		Renderer(Renderer&&) = delete;
		Renderer(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		[[nodiscard]] int GetRenderLayer() const { return m_RenderLayer; }
		void SetRenderLayer(int renderLayer) { m_RenderLayer = renderLayer; }

		void SetVisible(bool visible) { m_Visible = visible; }
		[[nodiscard]] bool IsVisible() const { return m_Visible; }

		// TODO: Should be protected
		virtual void Render() const {}
	protected:


	private:


		bool m_Visible;
		int m_RenderLayer{};

	};
}

