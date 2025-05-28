#include <Core/AssetManager.h>
#include <memory>
#include <iostream>
#include <SFML/Graphics/Texture.hpp>

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