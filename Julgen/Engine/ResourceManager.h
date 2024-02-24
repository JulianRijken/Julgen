#pragma once
#include "Singleton.h"

#include <filesystem>
#include <string>
#include <memory>

namespace jul
{
	class Texture2D;
	class Font;

	class ResourceManager final : public Singleton<ResourceManager>
	{
		friend class Singleton;

	public:
		void Init(const std::filesystem::path& data);

		[[nodiscard]] std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const;
		[[nodiscard]] std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;

	private:

		ResourceManager() = default;

		std::filesystem::path m_dataPath;
	};
}
