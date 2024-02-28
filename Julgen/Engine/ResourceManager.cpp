#include "ResourceManager.h"

#include <SDL_ttf.h>
#include <SDL_image.h>

#include "Font.h"
#include "RenderManager.h"
#include "Sprite.h"
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
}

//jul::Font* jul::ResourceManager::CreateFont(const std::string& assetName, const std::string& filePath, unsigned size)
//{
//	const auto fullPath = m_ContentPath / filePath;
//	return m_FontUPtrMap.emplace(assetName, std::make_unique<Font>(fullPath.string(), size)).first->second.get();
//}
//
//jul::Sprite* jul::ResourceManager::CreateSprite(const std::string& assetName, const std::string& filePath,
//	float pixelsPerUnit, const glm::vec2& pivotAlpha, int rowCount, int colCount,
//	const std::map<std::string, Animation>& animations)
//{
//	const auto fullPath = m_ContentPath / filePath;
//	return m_SpriteUPtrMap.emplace(assetName, std::make_unique<Sprite>(LoadTexture(filePath), pixelsPerUnit, pivotAlpha, rowCount, colCount, animations)).first->second.get();
//}



jul::Texture2D* jul::ResourceManager::LoadTexture(const std::string& filePath)
{
	const auto fullPath = m_ContentPath / filePath;
	SDL_Texture* texture = IMG_LoadTexture(RenderManager::GetInstance().GetSDLRenderer(), fullPath.string().c_str());

	if (texture == nullptr)
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());

	return m_LoadedTextureUPtrs.emplace_back(std::make_unique<Texture2D>(texture)).get();
}


