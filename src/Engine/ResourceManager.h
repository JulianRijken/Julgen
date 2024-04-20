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

		[[nodiscard]] static Font* GetFont(const std::string& name);
		[[nodiscard]] static Sprite* GetSprite(const std::string& name);

		static Font*   LoadFont   (const std::string& assetName, const std::string& filePath, unsigned int size);
		static Sprite* LoadSprite (const std::string& assetName, const std::string& filePath,
			int pixelsPerUnit = 100, const glm::vec2& pivotAlpha = {},
                                  int rowCount = 1, int colCount = 1, const std::map<std::string, SpriteAnimation>& animations = {});

	private:

		// Textures are currently only used by the sprites, so they are not exposed to the user
		// In the future when textures are needed for 3D models or other things, this should be changed
		static Texture2D* LoadTexture(const std::string& filePath);

		static void ConfigurePath();

        inline static std::map<std::string, std::unique_ptr<Font>> s_FontUPtrMap{};
        inline static std::map<std::string, std::unique_ptr<Sprite>> s_SpriteUPtrMap{};
        inline static std::vector<std::unique_ptr<Texture2D>> s_LoadedTextureUPtrs{};
        inline static std::filesystem::path s_ContentPath{};
    };
}


