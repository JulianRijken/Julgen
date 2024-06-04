#include "Transform.h"

#include <algorithm>

#include "fmt/core.h"
#include "GameObject.h"
#include "Rigidbody.h"


jul::Transform::Transform(GameObject* parent,const glm::vec3& position) :
	Component(parent,"Transform"),
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
	if (m_TransformDirty)
		UpdateWorldPosition();

	return m_WorldPosition;
}

void jul::Transform::SetLocalPosition(double x, double y, double z) { SetLocalPosition({ x, y, z }); }
void jul::Transform::SetLocalPosition(const glm::vec3& position)
{
    if(m_LocalPosition == position)
        return;

    m_LocalPosition = position;
    SetTransformDirty();

    if(m_Rigidbody)
        m_Rigidbody->SetPosition(GetWorldPosition());
}

void jul::Transform::SetWorldPosition(double x, double y, double z) { SetWorldPosition({ x, y, z }); }
void jul::Transform::SetWorldPosition(const glm::vec3& position)
{
	if (m_ParentPtr == nullptr)
		SetLocalPosition(position);
	else
        SetLocalPosition(position - m_ParentPtr->GetWorldPosition());
}

void jul::Transform::Translate(double x, double y, double z) { Translate({ x, y, z }); }

void jul::Transform::Translate(const glm::vec3& translation)
{
	SetLocalPosition(m_LocalPosition + translation);
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
            m_LocalPosition += m_ParentPtr->GetWorldPosition();
    }

    m_ParentPtr = newParentPtr;

    // Add to new parent
    if (m_ParentPtr != nullptr)
	{
		m_ParentPtr->m_ChildPtrs.insert(this);

		if (worldPositionStays)
            m_LocalPosition -= m_ParentPtr->GetWorldPosition();
    }

    SetTransformDirty();
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

    m_TransformDirty = false;
}

void jul::Transform::SetTransformDirty()
{
    m_TransformDirty = true;

    for(Transform* childPtr : m_ChildPtrs)
        if(not childPtr->m_TransformDirty)
            childPtr->SetTransformDirty();
}

void jul::Transform::OnRigidbodyDestroyed() { m_Rigidbody = nullptr; }
