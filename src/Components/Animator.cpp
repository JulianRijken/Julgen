#include "Animator.h"
#include "GameObject.h"
#include "MathExtensions.h"
#include "Sprite.h"

#include <GameTime.h>

jul::Animator::Animator(GameObject* parentPtr, SpriteRenderer* spriteRendererPtr, const std::string& defaultAnimation,
                        bool loop) :
    Component{ parentPtr, "Animator" },
    m_SpriteRendererPtr{ spriteRendererPtr }
{
    if(m_SpriteRendererPtr == nullptr)
        m_SpriteRendererPtr = parentPtr->GetComponent<SpriteRenderer>();

    if(not defaultAnimation.empty())
        Play(defaultAnimation, loop);
}

void jul::Animator::Play(const std::string& name, bool looping, float startFrameTime, float speedMultiplier)
{
	m_IsPlaying = true;
	m_IsLooping = looping;
	m_SpeedMultiplier = speedMultiplier;
	m_FrameTime = startFrameTime;
    m_ActiveAnimationName = name;

    m_ActiveAnimationPtr = m_SpriteRendererPtr->GetSprite()->GetAnimation(name);
}

void jul::Animator::Stop()
{
    m_IsPlaying = false;
    m_ActiveAnimationPtr = nullptr;
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
    if(m_ActiveAnimationPtr == nullptr)
        return;

    if (m_IsPlaying)
	{
        m_FrameTime += GameTime::GetDeltaTime<float>() *
                       (static_cast<float>(m_ActiveAnimationPtr->framesPerSecond) /
                        static_cast<float>(m_ActiveAnimationPtr->frameCount)) *
                       m_SpeedMultiplier;

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

    m_SpriteRendererPtr->SetDrawCell(m_ActiveAnimationPtr->GetCellFromNormalizedTime(m_FrameTime));
}
