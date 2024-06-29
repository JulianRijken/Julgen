#ifndef Resources_H
#define Resources_H
#include <filesystem>
#include <map>
#include <unordered_map>

#include "Font.h"
#include "Sound_System.h"
#include "Sprite.h"
#include "Texture2D.h"

namespace jul
{
    class Resources final
    {

    public:
        Resources() = delete;

        static void Initialize();
        static void Destroy();

        static std::filesystem::path GetAssetsPath() { return g_ContentPath; }

        [[nodiscard]] static Font* GetFont(const std::string& name);
        [[nodiscard]] static Sprite* GetSprite(const std::string& name);

        static Font* LoadFont(const std::string& assetName, const std::string& filePath, int size);
        static Sprite* LoadSprite(const std::string& assetName, const std::string& filePath, int pixelsPerUnit = 100,
                                  const glm::vec2& pivotAlpha = {}, int rowCount = 1, int colCount = 1,
                                  const std::map<std::string, SpriteAnimation>& animations = {});

        template<typename SoundType>
        static SoundWave* GetSound(SoundType soundType)
        {
            int soundId = static_cast<int>(soundType);

            if(not g_SoundPathsUPtrMap.contains(soundId))
                throw std::runtime_error("Sound Not Bound");

            if(g_SoundUPtrMap.contains(soundId))
                return g_SoundUPtrMap[soundId].get();

            const std::string& path = g_SoundPathsUPtrMap[soundId];
            return g_SoundUPtrMap.emplace(soundId, std::make_unique<SoundWave>(path)).first->second.get();
        }

        template<typename SoundType>
        static void BindSound(SoundType soundType, const std::string& filePath, bool preload = false)
        {
            int soundId = static_cast<int>(soundType);

            g_SoundPathsUPtrMap.emplace(soundId, (g_ContentPath / filePath).string());

            if(preload)
                GetSound(soundId);
        }


    private:
        // Textures are currently only used by the sprites, so they are not exposed to the user
		// In the future when textures are needed for 3D models or other things, this should be changed
        static Texture2D* LoadTexture(const std::string& filePath);

        static void ConfigurePath();

        inline static std::unordered_map<int, std::unique_ptr<SoundWave>> g_SoundUPtrMap{};
        inline static std::unordered_map<int, std::string> g_SoundPathsUPtrMap{};
        inline static std::map<std::string, std::unique_ptr<Font>> g_FontUPtrMap{};
        inline static std::map<std::string, std::unique_ptr<Sprite>> g_SpriteUPtrMap{};
        inline static std::vector<std::unique_ptr<Texture2D>> g_LoadedTextureUPtrs{};
        inline static std::filesystem::path g_ContentPath{};
    };
}  // namespace jul
#endif  // Resources_H
