#include "SceneManager.h"

#include <iostream>
#include <ranges>

#include "AutoMove.h"
#include "Bounce.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextRenderer.h"
#include "Transform.h"
#include "FpsCounter.h"
#include "SpriteRenderer.h"
#include "AutoRotateAround.h"
#include "GameObject.h"
#include "GlobalSettings.h"


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

		GameObject* assignment = AddGameObject("AssignmentText", { 135, 20, 0 });
		assignment->AddComponent<TextRenderer>("Programming 4 Scenegraph", font, 100);

		GameObject* fpsCounter = AddGameObject("Fps Counter", { 20,20,0 });
		fpsCounter->AddComponent<TextRenderer>("error", font, 100);
		fpsCounter->AddComponent<FpsCounter>();

		if (bool showPosition = true)
		{
			GameObject* bubble1 = AddGameObject("Bubble", { 300,250,0 });
			bubble1->AddComponent<SpriteRenderer>("Bubble.png", 10);
			bubble1->AddComponent<AutoRotateAround>(50.0f, 3.0f, bubble1->GetTransform().WorldPosition());

			GameObject* bubble2 = AddGameObject("Bubble", { 350,250,0 });
			bubble2->AddComponent<SpriteRenderer>("Bubble.png", 10);

			GameObject* bubble3 = AddGameObject("Bubble", { 400,250,0 });
			bubble3->AddComponent<SpriteRenderer>("Bubble.png", 10);



			bubble2->GetTransform().SetParent(&bubble1->GetTransform());
			bubble3->GetTransform().SetParent(&bubble2->GetTransform());

			bubble2->GetTransform().SetParent(nullptr);
			bubble2->GetTransform().SetParent(&bubble1->GetTransform());



			GameObject* bubble4 = AddGameObject("Bubble", { 10,10,0 });
			bubble4->AddComponent<SpriteRenderer>("Bubble.png", 10);

			bubble4->GetTransform().SetParent(&bubble3->GetTransform(), false);
			bubble4->GetTransform().SetParent(nullptr, false);


			bubble4->GetTransform().SetParent(&bubble3->GetTransform(), false);
			bubble4->GetTransform().SetParent(nullptr, true);
		}


		if (bool showChildStructure = false)
		{
			GameObject* bubble1 = AddGameObject("Bubble", { 300,250,0 });
			bubble1->AddComponent<SpriteRenderer>("Bubble.png", 10);
			bubble1->AddComponent<AutoRotateAround>(30.0f, 3.0f, bubble1->GetTransform().WorldPosition());

			GameObject* bubble2 = AddGameObject("Bubble", { 300,250,0 });
			bubble2->AddComponent<SpriteRenderer>("Bubble.png", 10);
			bubble2->AddComponent<AutoRotateAround>(70.0f, -3.0f);
			bubble2->GetTransform().SetParent(&bubble1->GetTransform());

			GameObject* bubble3 = AddGameObject("Bubble", { 300,250,0 });
			bubble3->AddComponent<SpriteRenderer>("Bubble.png", 10);
			bubble3->AddComponent<AutoRotateAround>(40.0f, 2.0f);
			bubble3->GetTransform().SetParent(&bubble2->GetTransform());
		}

		if  (bool showDirtyFlag = false)
		{
			GameObject* bubbleBase = AddGameObject("Bubble", { GlobalSettings::WINDOW_WIDTH / 2.0f,GlobalSettings::WINDOW_HEIGHT / 2.0f,0 });
			bubbleBase->AddComponent<SpriteRenderer>("Dot.png", 10);

			GameObject* lastBubble = bubbleBase;
			for (int i = 0; i < 20000; ++i)
			{
				GameObject* bubble = AddGameObject("Bubble", { 300,250,0 });
				bubble->AddComponent<SpriteRenderer>("Dot.png", 10);
				bubble->AddComponent<AutoRotateAround>(1.0f * (i * 0.05f), 1.0f + (i * 0.005f));
				bubble->GetTransform().SetParent(&lastBubble->GetTransform());

				lastBubble = bubble;
			}
		}

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

