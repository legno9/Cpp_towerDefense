#pragma once

#include <string>
#include <unordered_map>
#include <memory>


namespace sf
{
	class Texture;
	class Font;
}

class AssetManager
{
	public:

		static AssetManager& getInstance();

		sf::Texture& loadTexture(const std::string& assetPath);
		sf::Texture& getTexture(const std::string& assetPath) const;

		void unloadTexture(const std::string& assetPath);

		sf::Font& loadFont(const std::string& fontPath);
		sf::Font& getFont(const std::string& fontPath) const;

		void unloadFont(const std::string& fontPath);

		sf::Font& getDefaultFont();

	private:

		AssetManager() = default;
		~AssetManager() = default;

		AssetManager(const AssetManager&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;

		std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_texturePathToTexture {};
		std::unordered_map<std::string, std::unique_ptr<sf::Font>> m_loadedFonts {};
};