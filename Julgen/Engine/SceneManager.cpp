#include "SceneManager.h"

#include <iostream>

#include "GameObject.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "Timing.h"
#include "Transform.h"


void jul::SceneManager::LoadScene(const std::string& name)
{
	// Set new active scene
	// This should unload the last active scene
	m_ActiveScene = std::shared_ptr<Scene>(new Scene{ name });

	// TODO: Scenes are defined like this now and should be replaced by file loading
	if(name == "Start")
	{

		/// PROBLEM

		GameObject& background = AddGameObject("Background");
		background.AddComponent<SpriteRenderer>("background.tga");
		//SpriteRenderer test{};



		GameObject& logo = AddGameObject("Logo");
		logo.AddComponent<SpriteRenderer>("logo.tga");
		logo.GetTransform().SetPosition(216, 180,0);


		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

		GameObject& fpsCounter = AddGameObject("FPS Counter");
		REMOVEME = &fpsCounter.AddComponent<TextRenderer>("Time For Bed", font);
		fpsCounter.GetTransform().SetPosition(40, 40, 0);
	}


}

jul::GameObject& jul::SceneManager::AddGameObject(const std::string& name)
{
	const auto newGameObject = std::shared_ptr<GameObject>(new GameObject{name});
	m_ActiveScene->AddGameObjectToScene(newGameObject);

	// All game objects always have a transform
	newGameObject->m_TransformPtr = &newGameObject->AddComponent<Transform>();


	return *newGameObject;
}




void jul::SceneManager::Update()
{
	REMOVEME->SetText(std::to_string(static_cast<int>(std::round(1.0 / Time::GetDeltaTime()))));


	m_ActiveScene->Update();
}

void jul::SceneManager::LateUpdate()
{
}

void jul::SceneManager::FixedUpdate()
{

}

void jul::SceneManager::Render() const
{
	m_ActiveScene->Render();
}


