#pragma once
#include "Component.h"

#include "SpriteRenderer.h"
#include "Sprite.h"


namespace jul
{
    class Animator final : public Component
    {
    public:
        explicit Animator(GameObject* parentPtr, SpriteRenderer* spriteRendererPtr = nullptr,
                          const std::string& defaultAnimation = {});

        void Play(const std::string& name, bool looping = false, float startFrameTime = 0, float speedMultiplier = 1);
        void Stop();

        [[nodiscard]] bool IsPlaying() const { return m_IsPlaying; }
        [[nodiscard]] bool  IsActiveAnimation(const std::string& name) const {return m_ActiveAnimationName == name;}
        [[nodiscard]] float GetFrameTime() const { return m_FrameTime; }

        void SetLooping(bool isLooping);
        void SetPlaybackSpeed(float playbackSpeed);

    private:

        void Update() override;

        bool m_IsPlaying{ false };
        bool m_IsLooping{true};
        float m_FrameTime{ 0 };
        float m_SpeedMultiplier{ 1 };
        std::string m_ActiveAnimationName{};

        const SpriteAnimation* m_ActiveAnimationPtr{ nullptr };
        SpriteRenderer* m_SpriteRendererPtr{ nullptr };
    };

}
