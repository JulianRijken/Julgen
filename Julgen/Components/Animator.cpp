#include "Animator.h"
#include "GameObject.h"
#include "MathExtensions.h"
#include "Sprite.h"

#include <GameTime.h>

jul::Animator::Animator(GameObject* parentPtr, SpriteRenderer* spriteRendererPtr) :
    Component{parentPtr,"Animator"},
    m_SpriteRendererPtr{spriteRendererPtr}
{
    if(m_SpriteRendererPtr == nullptr)
        m_SpriteRendererPtr = parentPtr->GetComponent<SpriteRenderer>();
}

void jul::Animator::PlayAnimation(const std::string& name, bool looping, float startFrameTime, float speedMultiplier)
{
	m_IsPlaying = true;
	m_IsLooping = looping;
	m_SpeedMultiplier = speedMultiplier;
	m_FrameTime = startFrameTime;
    m_ActiveAnimationName = name;

    m_ActiveAnimation = m_SpriteRendererPtr->GetSprite()->GetAnimation(name);
}



void jul::Animator::SetLooping(bool isLooping)
{
	m_IsLooping = isLooping;
}

void jul::Animator::SetPlaybackSpeed(float playbackSpeed)
{
	m_SpeedMultiplier = playbackSpeed;
}


void jul::Animator::Update()
{
	if(m_ActiveAnimation == nullptr)
		return;

    if (m_IsPlaying)
	{
        m_FrameTime += GameTime::GetDeltaTimeF() * (m_ActiveAnimation->FRAMES_PER_SECOND / static_cast<float>(m_ActiveAnimation->FRAME_COUNT)) * m_SpeedMultiplier;

		if(m_IsLooping)
        {
            m_FrameTime = jul::math::ClampLoop01(m_FrameTime);
        }
		else
        {
            if (m_FrameTime > 1.0f or m_FrameTime < 0.0f)
                m_IsPlaying = false;
        }
	}

    m_SpriteRendererPtr->SetDrawCell(m_ActiveAnimation->GetCellFromNormalizedTime(m_FrameTime));
}
