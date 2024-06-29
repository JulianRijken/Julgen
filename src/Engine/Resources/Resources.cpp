#include "Resources.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

#include "RenderManager.h"


void jul::Resources::Initialize()
{
    ConfigurePath();

    if (TTF_Init() != 0)
        throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
}

void jul::Resources::Destroy()
{
    // Force cleanup because of order
    g_SoundUPtrMap.clear();
    g_FontUPtrMap.clear();
    g_SpriteUPtrMap.clear();
    g_LoadedTextureUPtrs.clear();
    TTF_Quit();
}

jul::Font* jul::Resources::GetFont(const std::string& name)
{
    if(g_FontUPtrMap.contains(name))
        return g_FontUPtrMap.at(name).get();

    std::cerr << "Font not loaded: " << name << '\n';
    return nullptr;
}

jul::Sprite* jul::Resources::GetSprite(const std::string& name)
{
    if(g_SpriteUPtrMap.contains(name))
        return g_SpriteUPtrMap.at(name).get();

    std::cerr << "Sprite not loaded: " << name << '\n';
    return nullptr;
}

jul::Font* jul::Resources::LoadFont(const std::string& assetName, const std::string& filePath, int size)
{
    assert(size > 0);
    const auto fullPath = g_ContentPath / filePath;
    return g_FontUPtrMap.emplace(assetName, std::make_unique<Font>(fullPath.string(), size)).first->second.get();
}

jul::Sprite* jul::Resources::LoadSprite(const std::string& assetName, const std::string& filePath,
	int pixelsPerUnit, const glm::vec2& pivotAlpha,
                                              int rowCount, int colCount, const std::map<std::string, SpriteAnimation>& animations)
{
    const auto fullPath = g_ContentPath / filePath;
    return g_SpriteUPtrMap
        .emplace(
            assetName,
            std::make_unique<Sprite>(LoadTexture(filePath), pixelsPerUnit, pivotAlpha, rowCount, colCount, animations))
        .first->second.get();
}


jul::Texture2D* jul::Resources::LoadTexture(const std::string& filePath)
{
    const auto fullPath = g_ContentPath / filePath;
    SDL_Texture* texture = IMG_LoadTexture(RenderManager::GetInstance().GetSDLRenderer(), fullPath.string().c_str());

    if(texture == nullptr)
        throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());


    return g_LoadedTextureUPtrs.emplace_back(std::make_unique<Texture2D>(texture)).get();
}

void jul::Resources::ConfigurePath()
{
#if __EMSCRIPTEN__
	m_ContentPath = "";
#else
    g_ContentPath = "./Assets/";

    // Check if the Content folder is in the parent directory
    if(not std::filesystem::exists(g_ContentPath))
        g_ContentPath = "../Assets/";
#endif

    if(not std::filesystem::exists(g_ContentPath))
    {
        std::cerr << "Content folder not found in directory: " << g_ContentPath << "\n"
                  << "Absolute Path: " << absolute(g_ContentPath) << '\n'
                  << "Current Program Path: " << std::filesystem::current_path() << '\n';
        throw std::runtime_error("Content folder not found in directory");
	}
}


