#pragma once
#include "Component.h"

namespace jul
{
	class RenderComponent : public Component
	{

	public:
		RenderComponent(int renderLayer = 0, bool visible = true, const std::string& name = "Renderer");
		~RenderComponent() override = default;

		RenderComponent(RenderComponent&&) = delete;
		RenderComponent(const RenderComponent&) = delete;
		RenderComponent& operator=(RenderComponent&&) = delete;
		RenderComponent& operator=(const RenderComponent&) = delete;

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

