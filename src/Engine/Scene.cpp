#include "Scene.h"
#include "GameObject.h"

jul::Scene::Scene(int id) :
    m_Id(id)
{
}

jul::GameObject* jul::Scene::AddGameObject(const std::string& name, const glm::vec3& position, const GameObject* parent,
                                           bool worldPositionStays)
{
    m_GameObjectsInSceneSPtr.emplace_back(std::make_unique<GameObject>(name, this, position));

    if(parent != nullptr)
        m_GameObjectsInSceneSPtr.back().get()->GetTransform().SetParent(&parent->GetTransform(), worldPositionStays);

    return m_GameObjectsInSceneSPtr.back().get();
}

void jul::Scene::Update() const
{
	for(const std::unique_ptr<GameObject>& gameObject : m_GameObjectsInSceneSPtr)
		gameObject->Update();
}

void jul::Scene::LateUpdate() const
{
	for (const std::unique_ptr<GameObject>& gameObject : m_GameObjectsInSceneSPtr)
		gameObject->LateUpdate();
}

void jul::Scene::FixedUpdate() const
{
	for (const std::unique_ptr<jul::GameObject>& gameObject : m_GameObjectsInSceneSPtr)
		gameObject->FixedUpdate();
}

void jul::Scene::CleanupGameObjects()
{
	// We propagate the destroy, only at the end of the frame!
	// This is very important:
	// - If we propagate the destroy immediately, we might end up destroying a game object whos parent has changes after calling the destroy
    for(auto& gameObject : m_GameObjectsInSceneSPtr)
        if(gameObject->IsBeingDestroyed())
            gameObject->PropagateDestroy();

    // Clean up individual components from game objects
    for(auto& gameObject : m_GameObjectsInSceneSPtr)
        gameObject->CleanupComponents();

    // Remove all game objects that are set to be destroyed
    for(auto iterator = m_GameObjectsInSceneSPtr.begin(); iterator != m_GameObjectsInSceneSPtr.end();)
        if ((*iterator)->IsBeingDestroyed())
			iterator = m_GameObjectsInSceneSPtr.erase(iterator);
		else
            ++iterator;
}

void jul::Scene::Unload()
{
    if(m_BeingUnloaded)
        return;

    Clean();

    m_BeingUnloaded = true;
}

void jul::Scene::Clean()
{
    for(auto& gameObject : m_GameObjectsInSceneSPtr)
    {
        gameObject->Destroy();
        gameObject->PropagateDestroy();
    }
}
