#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

#include "GameObject.h"
#include "Rigidbody.h"

jul::Transform::Transform(GameObject* parentPtr, const glm::vec3& position) :
    Component(parentPtr, "Transform"),
    m_LocalPosition(position)
{}

jul::Transform::~Transform()
{
	SetParent(nullptr);

	// Set parent will try to erase itself from the parent's children
	// This is NOT done with SetParent(nullptr) because
	// SetParent() will try to remove itself from m_ChildPtrs
	for (auto it = m_ChildPtrs.begin(); it != m_ChildPtrs.end();)
	{
		Transform* childPtr = *it;
		it = m_ChildPtrs.erase(it);
		childPtr->SetParent(nullptr);
	}
}

const glm::vec3& jul::Transform::GetWorldPosition()
{
    if(m_PositionDirty)
        UpdateWorldPosition();

    return m_WorldPosition;
}

const glm::quat& jul::Transform::GetWorldRotation()
{
    if(m_RotationDirty)
        UpdateWorldRotation();

    return m_WorldRotation;
}

const glm::vec3& jul::Transform::GetWorldScale()
{
    if(m_ScaleDirty)
        UpdateWorldScale();

    return m_WorldScale;
}

const glm::mat4& jul::Transform::GetWorldMatrix()
{
    if(m_MatrixDirty)
        UpdateWorldMatrix();

    return m_WorldMatrix;
}

glm::vec3 jul::Transform::GetEulerAngles() { return glm::degrees(glm::eulerAngles(GetWorldRotation())); }

void jul::Transform::SetLocalPosition(double x, double y, double z) { SetLocalPosition({ x, y, z }); }
void jul::Transform::SetLocalPosition(const glm::vec3& position)
{
    if(m_LocalPosition == position)
        return;

    m_LocalPosition = position;
    SetPositionDirty();

    if(m_Rigidbody)
        m_Rigidbody->SetPosition(GetWorldPosition());
}

void jul::Transform::SetLocalRotation(double x, double y, double z) { SetLocalRotation({ x, y, z }); }

void jul::Transform::SetLocalRotation(const glm::vec3& rotation)
{
    SetLocalRotation(glm::quat(glm::radians(rotation)));
}

void jul::Transform::SetLocalRotation(const glm::quat& rotation)
{
    if(m_LocalRotation == rotation)
        return;

    m_LocalRotation = rotation;
    SetRotationDirty();
}

void jul::Transform::SetLocalScale(double x, double y, double z) { SetLocalScale({ x, y, z }); }

void jul::Transform::SetLocalScale(const glm::vec3& scale)
{
    if(m_LocalScale == scale)
        return;

    m_LocalScale = scale;
    SetScaleDirty();
}

void jul::Transform::SetWorldPosition(double x, double y, double z) { SetWorldPosition({ x, y, z }); }
void jul::Transform::SetWorldPosition(const glm::vec3& position)
{
	if (m_ParentPtr == nullptr)
		SetLocalPosition(position);
	else
        SetLocalPosition(position - m_ParentPtr->GetWorldPosition());
}

void jul::Transform::SetWorldRotation(double x, double y, double z) { SetWorldRotation({ x, y, z }); }

void jul::Transform::SetWorldRotation(const glm::vec3& rotation)
{
    SetWorldRotation(glm::quat(glm::radians(rotation)));
}

void jul::Transform::SetWorldRotation(const glm::quat& rotation)
{
    if(m_ParentPtr == nullptr)
        SetLocalRotation(rotation);
    else
        SetLocalRotation(glm::inverse(m_ParentPtr->GetWorldRotation()) * rotation);
}

void jul::Transform::SetWorldScale(double x, double y, double z) { SetWorldScale({ x, y, z }); }

void jul::Transform::SetWorldScale(const glm::vec3& scale)
{

    if(m_ParentPtr == nullptr)
        SetLocalScale(scale);
    else
        SetLocalScale(scale - m_ParentPtr->GetWorldScale());
}


void jul::Transform::Translate(double x, double y, double z) { Translate({ x, y, z }); }

void jul::Transform::Translate(const glm::vec3& translation) { SetLocalPosition(m_LocalPosition + translation); }

void jul::Transform::Rotate(double x, double y, double z) { Rotate({ x, y, z }); }

void jul::Transform::Rotate(const glm::vec3& eulerAngles)
{
    SetLocalRotation(m_LocalRotation * glm::quat(glm::radians(eulerAngles)));
}


void jul::Transform::SetParent(Transform* newParentPtr, bool worldPositionStays)
{
	// Avoid: same parent - new parent being this - new parent being a child of this
	if (newParentPtr == m_ParentPtr or newParentPtr == this or IsChild(newParentPtr))
		return;

	// Remove from old parent
    if (m_ParentPtr != nullptr)
	{
		m_ParentPtr->m_ChildPtrs.erase(this);

		if (worldPositionStays)
        {
            m_LocalPosition += m_ParentPtr->GetWorldPosition();
            // TODO: SHOULD BE DONE FOR SCALE AND ROT
        }
    }

    m_ParentPtr = newParentPtr;

    // Add to new parent
    if (m_ParentPtr != nullptr)
	{
		m_ParentPtr->m_ChildPtrs.insert(this);

		if (worldPositionStays)
        {
            m_LocalPosition -= m_ParentPtr->GetWorldPosition();
            // TODO: SHOULD BE DONE FOR SCALE AND ROT
        }
    }

    SetPositionDirty();
    GetGameObject()->SetActiveDirty();
}

bool jul::Transform::IsChild(Transform* checkChildPtr) const
{
	if(m_ChildPtrs.contains(checkChildPtr))
		return true;

    return std::ranges::any_of(m_ChildPtrs, [=](const Transform* childPtr) {
        return childPtr->IsChild(checkChildPtr);
    });
}

void jul::Transform::SetRigidbody(Rigidbody* rigidbody)
{
    // There can't be more than one rigidbody
    assert(m_Rigidbody == nullptr);

    rigidbody->GetOnDestroyedEvent().AddListener(this, &Transform::OnRigidbodyDestroyed);
    m_Rigidbody = rigidbody;
}


void jul::Transform::UpdateWorldPosition()
{
	if(m_ParentPtr == nullptr)
		m_WorldPosition = m_LocalPosition;
	else
        m_WorldPosition = m_LocalPosition + m_ParentPtr->GetWorldPosition();

    m_PositionDirty = false;
}

void jul::Transform::UpdateWorldRotation()
{
    if(m_ParentPtr == nullptr)
        m_WorldRotation = m_LocalRotation;
    else  // * For quaternions
        m_WorldRotation = m_LocalRotation * m_ParentPtr->GetWorldRotation();

    m_RotationDirty = false;
}

void jul::Transform::UpdateWorldScale()
{
    if(m_ParentPtr == nullptr)
        m_WorldScale = m_LocalScale;
    else
        m_WorldScale = m_LocalScale * m_ParentPtr->GetWorldScale();

    m_ScaleDirty = false;
}

void jul::Transform::UpdateWorldMatrix()
{
    const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), GetWorldPosition());
    const glm::mat4 rotationMatrix = glm::mat4_cast(GetWorldRotation());
    const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), GetWorldScale());

    m_WorldMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    m_MatrixDirty = false;
}

void jul::Transform::SetPositionDirty()
{
    m_PositionDirty = true;
    m_MatrixDirty = true;

    for(Transform* childPtr : m_ChildPtrs)
        if(not childPtr->m_PositionDirty)
            childPtr->SetPositionDirty();
}

void jul::Transform::SetRotationDirty()
{
    m_RotationDirty = true;
    m_MatrixDirty = true;

    for(Transform* childPtr : m_ChildPtrs)
        if(not childPtr->m_RotationDirty)
            childPtr->SetRotationDirty();
}

void jul::Transform::SetScaleDirty()
{
    m_ScaleDirty = true;
    m_MatrixDirty = true;

    for(Transform* childPtr : m_ChildPtrs)
        if(not childPtr->m_ScaleDirty)
            childPtr->SetScaleDirty();
}

void jul::Transform::OnRigidbodyDestroyed(Object* /*unused*/) { m_Rigidbody = nullptr; }
