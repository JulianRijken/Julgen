#include <SDL.h>
#include <SDL_egl.h>

#if _DEBUG
	// ReSharper disable once CppUnusedIncludeDirective
	#if __has_include(<vld.h>)
	#include <vld.h>
	#endif
#endif


#include <filesystem>

#include "Julgen.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"



using namespace jul;

void load()
{
	Scene& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto gameObject = std::make_shared<GameObject>();
	gameObject->SetTexture("background.tga");
	scene.Add(gameObject);

	gameObject = std::make_shared<GameObject>();
	gameObject->SetTexture("logo.tga");
	gameObject->SetPosition(216, 180);
	scene.Add(gameObject);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	const auto text = std::make_shared<TextObject>("Julian Rijken", font);
	text->SetPosition(80, 20);
	scene.Add(text);
}

int main(int, char*[])
{
	Julgen engine{};
	engine.Run(load);
    return 0;
}



