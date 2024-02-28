#include "ResourceManager.h"

#include <iostream>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "RenderManager.h"
#include "Texture2D.h"


void jul::ResourceManager::Initialize()
{
#if __EMSCRIPTEN__
	m_ContentPath = "";
#else
	m_ContentPath = "./Content/";
#endif

	if (TTF_Init() != 0)
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());


	LoadFont("Lingua", "Lingua.otf", 36);
	LoadSprite("background", "background.tga", 32);
	LoadSprite("Bubble", "Bubble.png", 32);
	LoadSprite("Dot", "Dot.png", 32);
}

jul::Font* jul::ResourceManager::GetFont(const std::string& name)
{
	if (m_FontUPtrMap.contains(name))
		return m_FontUPtrMap.at(name).get();

	std::cerr << "Font not loaded: " << name << std::endl;
	return nullptr;
}

jul::Sprite* jul::ResourceManager::GetSprite(const std::string& name)
{
	if (m_SpriteUPtrMap.contains(name))
		return m_SpriteUPtrMap.at(name).get();

	std::cerr << "Sprite not loaded: " << name << std::endl;
	return nullptr;
}

jul::Font* jul::ResourceManager::LoadFont(const std::string& assetName, const std::string& filePath, unsigned size)
{
	const auto fullPath = m_ContentPath / filePath;
	return m_FontUPtrMap.emplace(assetName, std::make_unique<Font>(fullPath.string(), size)).first->second.get();
}

jul::Sprite* jul::ResourceManager::LoadSprite(const std::string& assetName, const std::string& filePath,
	int pixelsPerUnit, const glm::vec2& pivotAlpha,
	int rowCount, int colCount, const std::map<std::string, Animation>& animations)
{
	const auto fullPath = m_ContentPath / filePath;
	return m_SpriteUPtrMap.emplace(assetName, std::make_unique<Sprite>(LoadTexture(filePath), pixelsPerUnit, pivotAlpha, rowCount, colCount, animations)).first->second.get();
}



jul::Texture2D* jul::ResourceManager::LoadTexture(const std::string& filePath)
{
	const auto fullPath = m_ContentPath / filePath;
	SDL_Texture* texture = IMG_LoadTexture(RenderManager::GetInstance().GetSDLRenderer(), fullPath.string().c_str());

	if (texture == nullptr)
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());

	return m_LoadedTextureUPtrs.emplace_back(std::make_unique<Texture2D>(texture)).get();
}


