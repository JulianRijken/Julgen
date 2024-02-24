#include "FpsCounter.h"

#include "GameObject.h"
#include "TextRenderer.h"
#include "GameTime.h"

void FpsCounter::Update()
{
    //TODO: GetComponent is slow, cache the pointer
    jul::TextRenderer* textRenderer = GetGameObject()->GetComponent<jul::TextRenderer>();

    if(textRenderer == nullptr)
		return;
	

    std::stringstream text;
    text << std::fixed << std::setprecision(1) << std::floor(jul::GameTime::GetSmoothFps() * 10.0) / 10.0 << " FPS";

    textRenderer->SetText(text.str());
}
