#include "SceneManager.h"

#include <iostream>
#include <ranges>

#include "Bounce.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextRenderer.h"
#include "Transform.h"
#include "FpsCounter.h"
#include "SpriteRenderer.h"
#include "AutoMove.h"


void jul::SceneManager::LoadScene(const std::string& name)
{
	// Unload all loaded scenes
	m_LoadedScenes.clear();

	// Set new loaded scene as active
	m_ActiveScene = std::shared_ptr<Scene>(new Scene{ name });

	// TODO: Scenes are defined like this now and should be replaced by file loading
	if(name == "Start")
	{
		// TODO: Fonts should be stored in a resource manager and loaded by name
		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);


		GameObject* background = AddGameObject("Background");
		background->AddComponent<SpriteRenderer>("background.tga",-100);

		GameObject* assignment = AddGameObject("AssignmentText", { 100, 20, 0 });
		assignment->AddComponent<TextRenderer>("Programming 4 Assignment", font);


		for (int i = 0; i < 5; ++i)
		{
			const float offset = static_cast<float>(i) / 5.0f;

			GameObject* logo = AddGameObject("Logo", { -20 + offset * 50, 180, 0 });
			logo->AddComponent<SpriteRenderer>("logo.tga", -10);
			logo->AddComponent<Bounce>(50.0f * offset);
			logo->AddComponent<AutoMove>(glm::vec3{ 100,0,0 });
		}

		GameObject* fpsCounter = AddGameObject("Fps Counter", {40,40,0});
		fpsCounter->AddComponent<FpsCounter>();
		fpsCounter->AddComponent<TextRenderer>("error", font);
		fpsCounter->AddComponent<Bounce>(30.0f, 0.3f);
	}


	// Add scenes to loaded scenes
	m_LoadedScenes[name] = m_ActiveScene;
}

jul::GameObject* jul::SceneManager::AddGameObject(const std::string& name, const glm::vec3& position) const
{
	// TODO: I want to use make unique here but game has a private constructor
	auto newGameObject = std::unique_ptr<GameObject>(new GameObject(name, position));
	return m_ActiveScene->AddGameObjectToScene(std::move(newGameObject));
}




void jul::SceneManager::Update()
{
	for (const auto& scene : m_LoadedScenes | std::views::values)
		scene->Update();
}

void jul::SceneManager::LateUpdate()
{
	for (const auto& scene : m_LoadedScenes | std::views::values)
		scene->LateUpdate();
}

void jul::SceneManager::FixedUpdate()
{
	for (const auto& scene : m_LoadedScenes | std::views::values)
		scene->FixedUpdate();
}

void jul::SceneManager::Cleanup()
{
	for (const auto& scene : m_LoadedScenes | std::views::values)
		scene->Cleanup();
}

