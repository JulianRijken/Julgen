#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "Component.h"

namespace jul
{
class RenderComponent : public Component
	{
		friend class RenderManager;

	public:
        RenderComponent(GameObject* parentPtr, const std::string& name = "RenderComponent", int renderLayer = 0);
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
        int m_RenderLayer{};
    };
}

#endif // RENDERCOMPONENT_H