#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "RenderManager.h"
#include "Transform.h"

jul::Camera::Camera(GameObject* parent, float orthoSize, float aspectRatio, int priority) :
    Component(parent),
    m_Priority(priority),
    m_OrthoSize(orthoSize),
    m_AspectRatio(aspectRatio)
{
    RenderManager::g_Cameras.insert(this);
}

jul::Camera::~Camera() { RenderManager::g_Cameras.erase(this); }

glm::mat4 jul::Camera::GetViewPorjection() const
{
    const glm::mat4 projection =
        glm::ortho(-m_OrthoSize * m_AspectRatio, m_OrthoSize * m_AspectRatio, -m_OrthoSize, m_OrthoSize, -1.0f, 1.0f);

    const glm::mat4 view = glm::translate(glm::mat4(1.0f), GetTransform().GetWorldPosition());

    return projection * view;
}

void jul::Camera::SetPriority(int priority) { m_Priority = priority; }
