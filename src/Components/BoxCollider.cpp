#include "BoxCollider.h"

#include "GameObject.h"
#include "Locator.h"
#include "Physics.h"

jul::BoxCollider::BoxCollider(GameObject* parentPtr, const Settings& setting, Rigidbody* connectedBody) :
    Component(parentPtr, "Box Collider"),
    m_ConnectedRigidbody(connectedBody),
    m_Settings(setting)
{
    // Try finding Rigidbody when not manually connected
    if(connectedBody == nullptr)
    {
        // Try to find a rigidbody to connect to
        m_ConnectedRigidbody = GetGameObject()->GetComponentInParent<Rigidbody>();
    }

    Locator::Get<Physics>().AddCollider(this);
}

jul::BoxCollider::~BoxCollider() { Locator::Get<Physics>().RemoveCollider(this); }

void jul::BoxCollider::SetRestitution(float restitution) { m_Fixture->SetRestitution(restitution); }

void jul::BoxCollider::SetSensor(bool sensor) { m_Fixture->SetSensor(sensor); }
