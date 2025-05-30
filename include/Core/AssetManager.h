#pragma once

#include <string>
#include <unordered_map>
#include <memory>

namespace sf
{
	class Texture;
}

class AssetManager
{
	public:

		static AssetManager& getInstance();

		sf::Texture& loadTexture(const std::string& assetPath);
		sf::Texture& getTexture(const std::string& assetPath) const;

		void unloadTexture(const std::string& assetPath);

	private:

		AssetManager() = default;
		~AssetManager() = default;

		AssetManager(const AssetManager&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;

		std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_texturePathToTexture;
};