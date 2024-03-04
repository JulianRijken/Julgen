#pragma once
#include "Component.h"
#include "TextRenderer.h"


class FpsCounter final : public jul::Component
{
public:
	FpsCounter(jul::GameObject* parent);

private:
	void Awake() override;
	void Update() override;

	jul::TextRenderer* m_TextRenderer{};
};

