#include "FpsCounter.h"

#include <fmt/core.h>

#include "GameObject.h"
#include "TextRenderer.h"
#include "GameTime.h"


FpsCounter::FpsCounter(jul::GameObject* parent) : 
	Component(parent,"FpsCounter")
{
}

void FpsCounter::Awake()
{
    m_TextRenderer = GetGameObject()->GetComponent<jul::TextRenderer>();
}

void FpsCounter::Update()
{
    if (m_TextRenderer == nullptr)
        return;

    // TODO: (Remove this comment) Hey, Tom or Alex the SetText() uses dirty flag to avoid unnecessary updates ;)
    m_TextRenderer->SetText(fmt::format("{:.1f}", jul::GameTime::GetSmoothFps()));
}
