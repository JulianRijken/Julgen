#include "Scene.h"
#include "GameObject.h"

jul::Scene::Scene(int id) :
    m_Id(id)
{
}

jul::GameObject* jul::Scene::AddGameObject(const std::string& name, const glm::vec3& position, const GameObject* parent,
                                           bool worldPositionStays)
{
    m_GameObjectsAdded.emplace_back(std::make_unique<GameObject>(name, this, position));

    if(parent != nullptr)
        m_GameObjectsAdded.back().get()->GetTransform().SetParent(&parent->GetTransform(), worldPositionStays);

    return m_GameObjectsAdded.back().get();
}

void jul::Scene::Unload()
{
    if(m_BeingUnloaded)
        return;

    m_BeingUnloaded = true;
}

void jul::Scene::Update() const
{
    for(const std::unique_ptr<GameObject>& gameObject : m_GameObjectsInScene)
        if(gameObject->IsActiveInHierarchy())
            gameObject->Update();
}

void jul::Scene::LateUpdate() const
{
    for(const std::unique_ptr<GameObject>& gameObject : m_GameObjectsInScene)
        if(gameObject->IsActiveInHierarchy())
            gameObject->LateUpdate();
}

void jul::Scene::FixedUpdate() const
{
    for(const std::unique_ptr<jul::GameObject>& gameObject : m_GameObjectsInScene)
        if(gameObject->IsActiveInHierarchy())
            gameObject->FixedUpdate();
}

void jul::Scene::MoveGameObjectsAdded()
{
    std::move(m_GameObjectsAdded.begin(), m_GameObjectsAdded.end(), std::back_inserter(m_GameObjectsInScene));
    m_GameObjectsAdded.clear();
}

void jul::Scene::CleanupGameObjects()
{
    if(m_BeingUnloaded)
    {
        for(auto& gameObject : m_GameObjectsInScene)
        {
            if(not gameObject->IsBeingDestroyed())
            {
                gameObject->Destroy();
                gameObject->PropagateDestroy();
            }
        }

        m_GameObjectsInScene.clear();
    }
    else
    {
        for(auto& gameObject : m_GameObjectsInScene)
            if(gameObject->IsBeingDestroyed())
                gameObject->PropagateDestroy();

        // Clean up individual components from game objects
        for(auto& gameObject : m_GameObjectsInScene)
            gameObject->CleanupComponents();

        // Remove all game objects that are set to be destroyed
        for(auto iterator = m_GameObjectsInScene.begin(); iterator != m_GameObjectsInScene.end();)
            if((*iterator)->IsBeingDestroyed())
                iterator = m_GameObjectsInScene.erase(iterator);
            else
                ++iterator;
    }
}
