#pragma once
#include "Behaviour.h"
#include "TextRenderer.h"


class FpsCounter final : public jul::Behaviour
{
	void Awake() override;
	void Update() override;

	jul::TextRenderer* m_TextRenderer{};
};

