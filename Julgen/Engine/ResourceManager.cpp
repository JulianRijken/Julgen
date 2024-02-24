#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "ResourceManager.h"
#include "RenderManager.h"
#include "Texture2D.h"
#include "Font.h"

void jul::ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
}

std::shared_ptr<jul::Texture2D> jul::ResourceManager::LoadTexture(const std::string& file) const
{
	const auto fullPath = m_dataPath / file;
	auto texture = IMG_LoadTexture(RenderManager::GetInstance().GetSDLRenderer(), fullPath.string().c_str());

	if (texture == nullptr)
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());

	return std::make_shared<Texture2D>(texture);
}

std::shared_ptr<jul::Font> jul::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	const auto fullPath = m_dataPath/file;
	return std::make_shared<Font>(fullPath.string(), size);
}
