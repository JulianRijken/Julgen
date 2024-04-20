#include "ResourceManager.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

#include "RenderManager.h"
#include "Texture2D.h"

void jul::ResourceManager::Initialize()
{
    ConfigurePath();

    if (TTF_Init() != 0)
        throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
}

jul::Font* jul::ResourceManager::GetFont(const std::string& name)
{
    if(s_FontUPtrMap.contains(name))
        return s_FontUPtrMap.at(name).get();

    std::cerr << "Font not loaded: " << name << '\n';
    return nullptr;
}

jul::Sprite* jul::ResourceManager::GetSprite(const std::string& name)
{
    if(s_SpriteUPtrMap.contains(name))
        return s_SpriteUPtrMap.at(name).get();

    std::cerr << "Sprite not loaded: " << name << '\n';
    return nullptr;
}

jul::Font* jul::ResourceManager::LoadFont(const std::string& assetName, const std::string& filePath, unsigned size)
{
	const auto fullPath = s_ContentPath / filePath;
    return s_FontUPtrMap.emplace(assetName, std::make_unique<Font>(fullPath.string(), size)).first->second.get();
}

jul::Sprite* jul::ResourceManager::LoadSprite(const std::string& assetName, const std::string& filePath,
	int pixelsPerUnit, const glm::vec2& pivotAlpha,
                                              int rowCount, int colCount, const std::map<std::string, SpriteAnimation>& animations)
{
	const auto fullPath = s_ContentPath / filePath;
    return s_SpriteUPtrMap.emplace(assetName, std::make_unique<Sprite>(LoadTexture(filePath), pixelsPerUnit, pivotAlpha, rowCount, colCount, animations)).first->second.get();
}

jul::Texture2D* jul::ResourceManager::LoadTexture(const std::string& filePath)
{
	const auto fullPath = s_ContentPath / filePath;
	SDL_Texture* texture = IMG_LoadTexture(RenderManager::GetInstance().GetSDLRenderer(), fullPath.string().c_str());

	if (texture == nullptr)
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());


	return s_LoadedTextureUPtrs.emplace_back(std::make_unique<Texture2D>(texture)).get();
}

void jul::ResourceManager::ConfigurePath()
{
#if __EMSCRIPTEN__
	m_ContentPath = "";
#else
    s_ContentPath = "./Assets/";

	// Check if the Content folder is in the parent directory
	if (not std::filesystem::exists(s_ContentPath))
        s_ContentPath = "../Assets/";
#endif

	if (not std::filesystem::exists(s_ContentPath))
	{
		std::cerr << "Content folder not found in directory: " << s_ContentPath << "\n"
			<< "Absolute Path: " << absolute(s_ContentPath) << '\n'
			<< "Current Program Path: " << std::filesystem::current_path() << '\n';
		throw std::runtime_error("Content folder not found in directory");
	}
}


