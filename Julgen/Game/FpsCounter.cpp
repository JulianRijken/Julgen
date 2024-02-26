#include "FpsCounter.h"

#include "GameObject.h"
#include "TextRenderer.h"
#include "GameTime.h"


void FpsCounter::Awake()
{
    m_TextRenderer = GetGameObject()->GetComponent<jul::TextRenderer>();
}

void FpsCounter::Update()
{
    if (m_TextRenderer == nullptr)
        return;

    m_TextRenderer->SetText(std::format("{:.1f}", jul::GameTime::GetSmoothFps()));
}
