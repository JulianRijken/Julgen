#include "Transform.h"


jul::Transform::Transform(glm::vec3 position) :
	m_LocalPosition(position)
{}

const glm::vec3& jul::Transform::WorldPosition()
{
	if (m_PositionDirty)
		UpdateWorldPosition();

	return m_WorldPosition;
}


void jul::Transform::SetLocalPosition(float x, float y, float z)
{
	SetLocalPosition({ x,y,z });
}

void jul::Transform::SetLocalPosition(const glm::vec3& position)
{
	m_LocalPosition = position;
	SetPositionDirty();
}

void jul::Transform::SetWorldPosition(float x, float y, float z)
{
	SetWorldPosition({ x,y,z });
}

void jul::Transform::SetWorldPosition(const glm::vec3& position)
{
	if (m_ParentPtr == nullptr)
		SetLocalPosition(position);
	else
		SetLocalPosition(position - m_ParentPtr->WorldPosition());
}



void jul::Transform::Translate(float x, float y, float z)
{
	Translate({ x,y,z });
}

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
	if (m_ParentPtr)
	{
		m_ParentPtr->m_ChildPtrs.erase(this);

		if (worldPositionStays)
			m_LocalPosition += m_ParentPtr->WorldPosition();
	}

	m_ParentPtr = newParentPtr;

	// Add to new parent
	if (m_ParentPtr)
	{
		m_ParentPtr->m_ChildPtrs.insert(this);

		if (worldPositionStays)
			m_LocalPosition -= m_ParentPtr->WorldPosition();
	}
}

bool jul::Transform::IsChild(Transform* checkChildPtr) const
{
	if(m_ChildPtrs.contains(checkChildPtr))
		return true;

	for (const Transform* childPtr : m_ChildPtrs)
	{
		if (childPtr->IsChild(checkChildPtr))
			return true;
	}

	return false;
}

void jul::Transform::UpdateWorldPosition()
{
	if(m_ParentPtr == nullptr)
		m_WorldPosition = m_LocalPosition;
	else
		m_WorldPosition = m_LocalPosition + m_ParentPtr->WorldPosition();

	m_PositionDirty = false;
}

void jul::Transform::SetPositionDirty()
{
	m_PositionDirty = true;

	for (Transform* childPtr : m_ChildPtrs)
		if(not childPtr->m_PositionDirty)
			childPtr->SetPositionDirty();
}

