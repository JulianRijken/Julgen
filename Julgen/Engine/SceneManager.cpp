#include "SceneManager.h"

#include <Action.h>
#include <Input.h>
#include <iostream>
#include <ranges>

#include "ResourceManager.h"
#include "Scene.h"
#include "TextRenderer.h"
#include "Transform.h"
#include "FpsCounter.h"
#include "SpriteRenderer.h"
#include "AutoRotateAround.h"
#include "GameObject.h"
#include "GlobalSettings.h"
#include "Bounce.h"



void Test(int number)
{
    std::cout << "Action Triggered: " << number  << std::endl;
}


void jul::SceneManager::LoadScene(const std::string& name)
{
	// Unload all loaded scenes
    m_LoadedScenes.clear();


	// Set new loaded scene as active
	auto newSceneUPtr = std::make_unique<Scene>(name);
	m_ActiveScenePtr = newSceneUPtr.get();
	m_LoadedScenes[name] = std::move(newSceneUPtr);

	// TODO: Scenes are defined like this now and should be replaced by file loading
	if(name == "Start")
	{

		// TODO: This is not the correct wat to create game objects and components
		// This is still possible tho. Look in to this :)
		//[[maybe_unused]] GameObject* wrongGameObject = new GameObject("Test", glm::vec3(1, 1, 1));
		//[[maybe_unused]] FpsCounter* wrongFpsCounter = new FpsCounter(wrongGameObject);




        // Enemy
        Action<int> testAction{};


        // Plauer
        testAction.AddListener(Test);
        testAction.AddListener(BIND_MEMBER(SceneManager, TestMember));

        testAction.Invoke(1);

        testAction.RemoveListener(Test);
        testAction.RemoveListener(BIND_MEMBER(SceneManager, TestMember));

        testAction.Invoke(2);





        //auto testd = &SceneManager::TestMember;

        // testAction += Test;
        // ACTION_BIND2(testAction,SceneManager,TestMember);

        // testAction += {this, "TestMember", &SceneManager::TestMember};
        // testAction += { ACTION_BIND(SceneManager, TestMember) };
        //testAction.RemoveListener<SceneManager>(this, &SceneManager::TestMember2);
        // testAction.Invoke(2);



        if constexpr (constexpr bool showInput = false)
        {

            GameObject* background = AddGameObject("Background");
            background->AddComponent<SpriteRenderer>(ResourceManager::GetSprite("background"), -100);

            GameObject* assignment = AddGameObject("AssignmentText", { 135, 20, 0 });
            assignment->AddComponent<TextRenderer>("Programming 4 Scene-graph", ResourceManager::GetFont("Lingua"), 100);

            GameObject* fpsCounter = AddGameObject("Fps Counter", { 20,20,0 });
            fpsCounter->AddComponent<TextRenderer>("error", ResourceManager::GetFont("Lingua"), 100);
            fpsCounter->AddComponent<FpsCounter>();

            GameObject* inputInfoText1 = AddGameObject("Input Text 1", { 20,60,0 });
            inputInfoText1->AddComponent<TextRenderer>("Use the D-Pad or WASD to move Bubble", ResourceManager::GetFont("LinguaSmall"), 100);

            GameObject* inputInfoText2 = AddGameObject("Input Text 2", { 20,85,0 });
            inputInfoText2->AddComponent<TextRenderer>("Use the A button or SPACE to jump Bubble", ResourceManager::GetFont("LinguaSmall"), 100);

            GameObject* inputInfoText3 = AddGameObject("Input Text 2", { 20,110,0 });
            inputInfoText3->AddComponent<TextRenderer>("P1 = Keyboard, P2 = Gamepad1, P3 = Gamepad2", ResourceManager::GetFont("LinguaSmall"), 100);


            GameObject* bubbleCharacter1 = AddGameObject("Bubble", { 400,250,0 });
            bubbleCharacter1->AddComponent<SpriteRenderer>(ResourceManager::GetSprite("Bubble"), 0);
            GameObject* bubbleCharacter1PlayerText = AddGameObject("BubbleText", { 0,-40,0 });
            bubbleCharacter1PlayerText->GetTransform().SetParent(&bubbleCharacter1->GetTransform(),false);
            bubbleCharacter1PlayerText->AddComponent<TextRenderer>("P1", ResourceManager::GetFont("LinguaSmall"), 100);


            GameObject* bubbleCharacter2 = AddGameObject("Bubble", { 350,250,0 });
            bubbleCharacter2->AddComponent<SpriteRenderer>(ResourceManager::GetSprite("Bubble"), 0);
            GameObject* bubbleCharacter2PlayerText = AddGameObject("BubbleText", { 0,-40,0 });
            bubbleCharacter2PlayerText->GetTransform().SetParent(&bubbleCharacter2->GetTransform(),false);
            bubbleCharacter2PlayerText->AddComponent<TextRenderer>("P2", ResourceManager::GetFont("LinguaSmall"), 100);


            GameObject* bubbleCharacter3 = AddGameObject("Bubble", { 300,250,0 });
            bubbleCharacter3->AddComponent<SpriteRenderer>(ResourceManager::GetSprite("Bubble"), 0);
            GameObject* bubbleCharacter3PlayerText = AddGameObject("BubbleText", { 0,-40,0 });
            bubbleCharacter3PlayerText->GetTransform().SetParent(&bubbleCharacter3->GetTransform(),false);
            bubbleCharacter3PlayerText->AddComponent<TextRenderer>("P3", ResourceManager::GetFont("LinguaSmall"), 100);



            //TODO: This should idealy not be setup in the scene manager but it's here as a demonstation
            Input::GetInstance().RegisterCommand<MoveCommand>("moveKeyboardLeft"   ,ButtonState::Held,bubbleCharacter1,300.0f,glm::vec3{-1, 0, 0});
            Input::GetInstance().RegisterCommand<MoveCommand>("moveKeyboardRight"  ,ButtonState::Held,bubbleCharacter1,300.0f,glm::vec3{ 1, 0, 0});
            Input::GetInstance().RegisterCommand<MoveCommand>("moveKeyboardDown"   ,ButtonState::Held,bubbleCharacter1,300.0f,glm::vec3{ 0, 1, 0});
            Input::GetInstance().RegisterCommand<MoveCommand>("moveKeyboardUp"     ,ButtonState::Held,bubbleCharacter1,300.0f,glm::vec3{ 0,-1, 0});
            Input::GetInstance().RegisterCommand<UnitMoveCommand>("jumpKeyboard"   ,ButtonState::Down,bubbleCharacter1,       glm::vec3{ 0,-1, 0});
            Input::GetInstance().RegisterCommand<UnitMoveCommand>("jumpKeyboard"   ,ButtonState::Up,  bubbleCharacter1,       glm::vec3{ 0, 1, 0});

            Input::GetInstance().RegisterCommand<MoveCommand>("moveLeftController" ,ButtonState::Held,0,bubbleCharacter2,100.0f,glm::vec3{-1, 0, 0});
            Input::GetInstance().RegisterCommand<MoveCommand>("moveRightController",ButtonState::Held,0,bubbleCharacter2,100.0f,glm::vec3{ 1, 0, 0});
            Input::GetInstance().RegisterCommand<MoveCommand>("moveDownController" ,ButtonState::Held,0,bubbleCharacter2,100.0f,glm::vec3{ 0, 1, 0});
            Input::GetInstance().RegisterCommand<MoveCommand>("moveUpController"   ,ButtonState::Held,0,bubbleCharacter2,100.0f,glm::vec3{ 0,-1, 0});
            Input::GetInstance().RegisterCommand<UnitMoveCommand>("jumpController" ,ButtonState::Down,0,bubbleCharacter2,       glm::vec3{ 0,-1, 0});
            Input::GetInstance().RegisterCommand<UnitMoveCommand>("jumpController" ,ButtonState::Up,  0,bubbleCharacter2,       glm::vec3{ 0, 1, 0});

            Input::GetInstance().RegisterCommand<MoveCommand>("moveLeftController" ,ButtonState::Held,1,bubbleCharacter3,200.0f,glm::vec3{-1, 0, 0});
            Input::GetInstance().RegisterCommand<MoveCommand>("moveRightController",ButtonState::Held,1,bubbleCharacter3,200.0f,glm::vec3{ 1, 0, 0});
            Input::GetInstance().RegisterCommand<MoveCommand>("moveDownController" ,ButtonState::Held,1,bubbleCharacter3,200.0f,glm::vec3{ 0, 1, 0});
            Input::GetInstance().RegisterCommand<MoveCommand>("moveUpController"   ,ButtonState::Held,1,bubbleCharacter3,200.0f,glm::vec3{ 0,-1, 0});
            Input::GetInstance().RegisterCommand<UnitMoveCommand>("jumpController" ,ButtonState::Down,1,bubbleCharacter3,       glm::vec3{ 0,-1, 0});
            Input::GetInstance().RegisterCommand<UnitMoveCommand>("jumpController" ,ButtonState::Up,  1,bubbleCharacter3,       glm::vec3{ 0, 1, 0});


            Input::GetInstance().RegisterCommand<StickTestCommand>("stickExample" ,ButtonState::Held, 0);
            Input::GetInstance().RegisterCommand<TriggerTestCommand>("triggerExample" ,ButtonState::Held, 0);

        }
		if constexpr (constexpr bool showSceneGraph = false)
		{
			std::cout << "\n\n";

			/////////////////////////////
			/// Showing fix delta time clamp
			/////////////////////////////
			GameObject* bounce1 = AddGameObject("BubbleBounce", { 10,0,0 });
			bounce1->AddComponent<SpriteRenderer>(ResourceManager::GetSprite("Bubble"), 10000);
            bounce1->AddComponent<Bounce>(32.0f, 0.8f);


			/////////////////////////////
			/// Showing cleanup / destroy
			/////////////////////////////
			GameObject* deleteBubble = AddGameObject("Bubble", { 300,250,0 });
			deleteBubble->AddComponent<SpriteRenderer>(ResourceManager::GetSprite("Bubble"), 10000);

			GameObject* deleteBubbleChild = AddGameObject("Bubble", { 300,250,0 });
			deleteBubbleChild->AddComponent<SpriteRenderer>(ResourceManager::GetSprite("Bubble"), 10000);

			// Even though we call destroy before the child is set the child will be destroyed
			// Propagating children will only happen at the end of the frame
			deleteBubble->Destroy();
			deleteBubbleChild->GetTransform().SetParent(&deleteBubble->GetTransform());


			////////////////////////
			/// Showing triple child
			////////////////////////
			GameObject* bubble1 = AddGameObject("Bubble1", { 300,250,0 });
			bubble1->AddComponent<SpriteRenderer>(ResourceManager::GetSprite("Bubble"), 10000);
			bubble1->AddComponent<AutoRotateAround>(30.0f, 3.0f, bubble1->GetTransform().WorldPosition());

			// Changing parent will take effect immediately
			// But destroying will only happen at the end of the frame so buuble1 will not be destroyed
			bubble1->GetTransform().SetParent(&deleteBubble->GetTransform());
			bubble1->GetTransform().SetParent(&deleteBubbleChild->GetTransform());
			bubble1->GetTransform().SetParent(nullptr);


			GameObject* bubble2 = AddGameObject("Bubble2", { 300,250,0 });
			bubble2->AddComponent<SpriteRenderer>(ResourceManager::GetSprite("Bubble"), 10000);
			bubble2->AddComponent<AutoRotateAround>(70.0f, -3.0f);
			bubble2->GetTransform().SetParent(&bubble1->GetTransform());

			GameObject* bubble3 = AddGameObject("Bubble3", { 300,250,0 });
			bubble3->AddComponent<SpriteRenderer>(ResourceManager::GetSprite("Bubble"), 10000);
			bubble3->AddComponent<AutoRotateAround>(40.0f, 2.0f);
			bubble3->GetTransform().SetParent(&bubble2->GetTransform());


			////////////////////////
			/// Showing child spiral
			////////////////////////
			GameObject* bubbleBase = AddGameObject("Bubble", { 50,GlobalSettings::WINDOW_HEIGHT / 2.0f,0 });
			bubbleBase->AddComponent<SpriteRenderer>(ResourceManager::GetSprite("Dot"), 1);
			bubbleBase->AddComponent<AutoRotateAround>(40.0f, 1.0f);

			bubbleBase->GetTransform().SetParent(&bubble3->GetTransform());
			const GameObject* lastBubble = bubbleBase;
			for (int i = 0; i < 500; ++i)
			{
				GameObject* bubble = AddGameObject("Bubble", { 300,250,0 });
				bubble->AddComponent<SpriteRenderer>(ResourceManager::GetSprite("Dot"), i);
				bubble->AddComponent<AutoRotateAround>(1.0f * (i * 0.05f), 1.0f + (i * 0.005f));
				bubble->GetTransform().SetParent(&lastBubble->GetTransform());

				// Destroy every other bubble!
				// What it does not break!
				// Yes :) It does not break
				if (i % 2 == 0)
					bubble->Destroy();
				else
					lastBubble = bubble;
			}
		}
	}
}

jul::GameObject* jul::SceneManager::AddGameObject(const std::string& name, const glm::vec3& position) const
{
    return m_ActiveScenePtr->AddGameObjectToScene(std::make_unique<GameObject>(name, position));
}

void jul::SceneManager::TestMember(int number)
{
    std::cout << "Member function - Action Triggered: " << number  << std::endl;
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

void jul::SceneManager::CleanupGameObjects()
{
	for (const auto& scene : m_LoadedScenes | std::views::values)
		scene->CleanupGameObjects();
}

