#include <Core/RenderManager.h>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>


RenderManager* RenderManager::s_instance = nullptr;

RenderManager::RenderManager(sf::RenderWindow& window)
    : m_window(window)
{}

RenderManager::~RenderManager()
{
    clearRenderQueue();
}


void RenderManager::initialize(sf::RenderWindow& window) 
{
    if (s_instance == nullptr) 
    {
        s_instance = new RenderManager(window);
    } 
    else 
    {
        std::cerr << "WARNING: RenderManager::initialize() called more than once." << std::endl;
    }
}

RenderManager& RenderManager::getInstance() 
{
    if (s_instance == nullptr) 
    {
        std::cerr << "ERROR: RenderManager::getInstance() called before initialize()." << std::endl;
        throw std::runtime_error("RenderManager not initialized!");
    }
    return *s_instance;
}

void RenderManager::render()
{
    for (const auto& pair : m_renderLayers)
    {
        const auto& layer = pair.second;
        for (const auto& drawable : layer->drawables)
        {
            m_window.draw(drawable.get());
        }
    }
}

bool RenderManager::addToRenderQueue(sf::Drawable& drawable, ZOrder zOrder)
{
    auto layersIt = m_renderLayers.find(zOrder);

    if (layersIt == m_renderLayers.end())
    {
        m_renderLayers[zOrder] = std::make_unique<renderLayer>(drawable);
    }
    else
    {
        auto& layerDrawables = layersIt->second->drawables;

        if (std::any_of(layerDrawables.begin(), layerDrawables.end(),
            [&drawable](const std::reference_wrapper<sf::Drawable>& d) { return &d.get() == &drawable; }))
        {
            std::cerr << "ERROR: RenderManager::addToRenderQueue() drawable already exists in the queue." << std::endl;
            return false; // Drawable already exists in the queue
        }

        layerDrawables.push_back(std::ref(drawable));
    }

    return true;
}

bool RenderManager::removeFromRenderQueue(sf::Drawable& drawable, ZOrder zOrder)
{
    auto layersIt = m_renderLayers.find(zOrder);

    if (layersIt == m_renderLayers.end())
    {
        return false;
    }
    
    auto& layerDrawables = layersIt->second->drawables;

    auto drawableIt = std::find_if(layerDrawables.begin(), layerDrawables.end(),
                    [&drawable](std::reference_wrapper<sf::Drawable> existingRef) 
                    {
                        return &existingRef.get() == &drawable;
                    });


    if (drawableIt == layerDrawables.end())
    {
        return false;
    }

    layerDrawables.erase(drawableIt);

    if (layerDrawables.empty())
    {
        m_renderLayers.erase(layersIt); // Remove the layer if it has no drawables left
    }

    return true;
}

void RenderManager::clearRenderQueue()
{
    m_renderLayers.clear();
}