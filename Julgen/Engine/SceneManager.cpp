#include "SceneManager.h"
#include "Scene.h"

void jul::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void jul::SceneManager::LateUpdate()
{
}

void jul::SceneManager::FixedUpdate()
{

}

void jul::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

jul::Scene& jul::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
