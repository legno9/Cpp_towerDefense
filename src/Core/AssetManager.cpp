#include <Core/AssetManager.h>
#include <memory>
#include <iostream>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

AssetManager& AssetManager::getInstance()
{
    static AssetManager instance;
    return instance;
}

sf::Texture& AssetManager::loadTexture(const std::string& assetPath)
{
	auto it = m_texturePathToTexture.find(assetPath);

	if (it != m_texturePathToTexture.end())
	{
		return *it->second;
	}
	else
	{
		auto newTexture = std::make_unique<sf::Texture>();
		if (!newTexture->loadFromFile(assetPath)) 
		{
			std::cerr << "ERROR: Failed to load texture: " << assetPath << std::endl;
			throw std::runtime_error("Failed to get texture: " + assetPath);
		}

        sf::Texture* rawPtr = newTexture.get();
        m_texturePathToTexture[assetPath] = std::move(newTexture);
        return *rawPtr;
	}
}

sf::Texture& AssetManager::getTexture(const std::string& assetPath) const
{
	const auto it = m_texturePathToTexture.find(assetPath);
	if (it == m_texturePathToTexture.end())
	{
		std::cerr << "ERROR: Failed to get texture: " << assetPath << std::endl;
		throw std::runtime_error("Failed to get texture: " + assetPath);
	}
	return *it->second;
}

void AssetManager::unloadTexture(const std::string& assetPath)
{
	auto it = m_texturePathToTexture.find(assetPath);
	if (it != m_texturePathToTexture.end())
	{
		m_texturePathToTexture.erase(it);
	}
	else
	{
		std::cerr << "ERROR: Failed to unload texture: " << assetPath << std::endl;
		throw std::runtime_error("Failed to load texture: " + assetPath);
	}
}

sf::Font& AssetManager::loadFont(const std::string& fontPath) 
{
    auto it = m_loadedFonts.find(fontPath);
    if (it != m_loadedFonts.end()) 
	{
        return *it->second;
    }

    auto newFont = std::make_unique<sf::Font>();
    if (!newFont->loadFromFile(fontPath)) 
	{
        std::cerr << "ERROR: FontManager failed to load font from: " << fontPath << std::endl;
        throw std::runtime_error("Failed to load font: " + fontPath);
    }
    
    sf::Font* rawPtr = newFont.get();
    m_loadedFonts[fontPath] = std::move(newFont);
    return *rawPtr;
}

sf::Font& AssetManager::getFont(const std::string& fontPath) const 
{
    auto it = m_loadedFonts.find(fontPath);
    if (it == m_loadedFonts.end() || it->second == nullptr) 
	{
        std::cerr << "ERROR: FontManager: Font '" << fontPath << "' not found in cache." << std::endl;
        throw std::runtime_error("Font not cached: " + fontPath);
    }
    return *it->second;
}

void AssetManager::unloadFont(const std::string& fontPath) 
{
	auto it = m_loadedFonts.find(fontPath);
	if (it != m_loadedFonts.end()) 
	{
		m_loadedFonts.erase(it);
	} 
	else 
	{
		std::cerr << "ERROR: FontManager: Failed to unload font '" << fontPath << "'." << std::endl;
		throw std::runtime_error("Failed to unload font: " + fontPath);
	}
}

sf::Font& AssetManager::getDefaultFont() 
{
	return loadFont("./data/fonts/Minecraft.ttf");
}