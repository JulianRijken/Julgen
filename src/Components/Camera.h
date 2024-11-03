#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>

#include "Component.h"


namespace jul
{
    class Camera final : public Component
    {
    public:
        Camera(GameObject* parent, float orthoSize, float aspectRatio, int priority = 0);

        ~Camera() override;

        Camera(Camera&&) = delete;
        Camera(const Camera&) = delete;
        Camera& operator=(Camera&&) = delete;
        Camera& operator=(const Camera&) = delete;

        [[nodiscard]] int GetPriority() const { return m_Priority; }

        [[nodiscard]] glm::mat4 GetViewPorjection() const;

        void SetPriority(int priority);

    private:
        int m_Priority{ 0 };
        float m_OrthoSize{ 14 };
        float m_AspectRatio{};
    };

}  // namespace jul
#endif // CAMERA_H
