#include <SDL.h>

#if _DEBUG
	// ReSharper disable once CppUnusedIncludeDirective
	#if __has_include(<vld.h>)
	#include <vld.h>
	#endif
#endif

#include "Julgen.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"

#include <filesystem>


void load()
{
	dae::Scene& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto gameObject = std::make_shared<dae::GameObject>();
	gameObject->SetTexture("background.tga");
	scene.Add(gameObject);

	gameObject = std::make_shared<dae::GameObject>();
	gameObject->SetTexture("logo.tga");
	gameObject->SetPosition(216, 180);
	scene.Add(gameObject);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	const auto to = std::make_shared<dae::TextObject>("Julian Rijken", font);

	to->SetPosition(80, 20);
	scene.Add(to);
}

int main(int, char*[])
{

#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	std::filesystem::path data_location = "./Content/";
	if(not exists(data_location))
		data_location = "../Content/";
#endif


	dae::Julgen engine(data_location);
	engine.Run(load);

    return 0;
}



