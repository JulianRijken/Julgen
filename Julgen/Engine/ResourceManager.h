#pragma once
#include <filesystem>
#include <map>

#include "Font.h"
#include "Sprite.h"
#include "Texture2D.h"


namespace jul
{

	class ResourceManager final
	{

	public:
		static void Initialize();

		//[[nodiscard]] static Font*   CreateFont   (const std::string& assetName, const std::string& filePath, unsigned int size);
		//[[nodiscard]] static Sprite* CreateSprite (const std::string& assetName, const std::string& filePath,
		//	float pixelsPerUnit, const glm::vec2& pivotAlpha, int rowCount,
		//	int colCount, const std::map<std::string, Animation>& animations);

	private:

		[[nodiscard]] static Texture2D* LoadTexture(const std::string& filePath);


		ResourceManager() = default;

		inline static std::map<std::string, std::unique_ptr<Font>> m_FontUPtrMap;
		inline static std::map<std::string, std::unique_ptr<Sprite>> m_SpriteUPtrMap;

		inline static std::vector<std::unique_ptr<Texture2D>> m_LoadedTextureUPtrs;

		inline static std::filesystem::path m_ContentPath;
	};
}


