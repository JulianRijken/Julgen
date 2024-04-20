#pragma once

#include "Component.h"

namespace jul
{
class RenderComponent : public Component
	{
		friend class RenderManager;

	public:
        RenderComponent(GameObject* parent, const std::string& name = "RenderComponent", int renderLayer = 0);
        ~RenderComponent() override;

        RenderComponent(RenderComponent&&) = delete;
        RenderComponent(const RenderComponent&) = delete;
        RenderComponent& operator=(RenderComponent&&) = delete;
        RenderComponent& operator=(const RenderComponent&) = delete;

		[[nodiscard]] int GetRenderLayer() const { return m_RenderLayer; }
		void SetRenderLayer(int renderLayer) { m_RenderLayer = renderLayer; }


	protected:

		virtual void Render() const {}
		virtual void UpdateGUI() {}

	private:

		bool m_Visible{true};
		int m_RenderLayer{};
	};
}

