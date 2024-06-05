#include "Scene.h"
#include "GameObject.h"

jul::Scene::Scene(int id) :
    m_Id(id)
{
}

jul::GameObject* jul::Scene::AddGameObject(const std::string& name, const glm::vec3& position, const GameObject* parent,
                                           bool worldPositionStays)
{
    n_GameObjectsAddedPriviousFrame.emplace_back(std::make_unique<GameObject>(name, this, position));

    if(parent != nullptr)
        n_GameObjectsAddedPriviousFrame.back().get()->GetTransform().SetParent(&parent->GetTransform(),
                                                                               worldPositionStays);

    return n_GameObjectsAddedPriviousFrame.back().get();
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
        gameObject->FixedUpdate();
}

void jul::Scene::AddNewGameObjects()
{
    std::move(n_GameObjectsAddedPriviousFrame.begin(),
              n_GameObjectsAddedPriviousFrame.end(),
              std::back_inserter(m_GameObjectsInScene));
    n_GameObjectsAddedPriviousFrame.clear();
}

void jul::Scene::CleanupGameObjects()
{
	// We propagate the destroy, only at the end of the frame!
	// This is very important:
	// - If we propagate the destroy immediately, we might end up destroying a game object whos parent has changes after calling the destroy
    for(auto& gameObject : m_GameObjectsInScene)
        if(gameObject->IsBeingDestroyed())
            gameObject->PropagateDestroy();

    // Clean up individual components from game objects
    for(auto& gameObject : m_GameObjectsInScene)
        gameObject->CleanupComponents();

    // Remove all game objects that are set to be destroyed
    for(auto iterator = m_GameObjectsInScene.begin(); iterator != m_GameObjectsInScene.end();)
        if ((*iterator)->IsBeingDestroyed())
            iterator = m_GameObjectsInScene.erase(iterator);
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
    for(auto& gameObject : m_GameObjectsInScene)
    {
        gameObject->Destroy();
        gameObject->PropagateDestroy();
    }
}
