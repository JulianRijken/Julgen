#pragma once
#include "Component.h"

#include "SpriteRenderer.h"
#include "Sprite.h"


namespace jul
{
    class Animator final : public Component
    {
    public:

        Animator(GameObject* parent, SpriteRenderer* spriteRendererPtr);

        void PlayAnimation(const std::string& name, bool looping = false, float startFrameTime = 0, float speedMultiplier = 1);

        [[nodiscard]] bool GetIsPlaying() const { return m_IsPlaying; }
        [[nodiscard]] float GetFrameTime() const { return m_FrameTime; }

        void SetLooping(bool isLooping);
        void SetPlaybackSpeed(float playbackSpeed);

    private:

        void Update() override;

        bool m_IsPlaying = false;
        bool m_IsLooping = true;
        float m_FrameTime = 0;
        float m_SpeedMultiplier = 1;

        const SpriteAnimation* m_ActiveAnimation = nullptr;
        SpriteRenderer* m_SpriteRendererPtr; // Non Owning
    };

}
