
#include <cassert>
#include <Core/RenderManager.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>


RenderManager::RenderManager(sf::RenderWindow* window)
{
    assert(window != nullptr && "RenderManager::RenderManager() window is nullptr");
    m_window = window;
}

RenderManager::~RenderManager()
{
    m_window = nullptr;
    clearRenderQueue();
}

void RenderManager::render()
{
    assert(m_window != nullptr && "RenderManager::RenderManager() m_window is nullptr");

    for (const auto& layer : renderQueue)
    {
        for (const auto& drawable : layer->drawables)
        {
            m_window->draw(*drawable);
        }
    }
}

bool RenderManager::addToRenderQueue(sf::Drawable* drawable, ZOrder zOrder)
{
    assert(drawable != nullptr && "RenderManager::addToRender() drawable is nullptr");

    auto layersIt = std::find_if(renderQueue.begin(), renderQueue.end(), [zOrder](const std::unique_ptr<renderLayer>& layer)
    {
        return layer->zOrder == zOrder;
    });

    if (layersIt != renderQueue.end())
    {
        auto drawableIt = FindDrawable(drawable, *layersIt);
        
        if (drawableIt == layersIt->get()->drawables.end())
        {
            layersIt->get()->drawables.push_back(drawable);
        }
        else{return false;}
    }
    else
    {
        auto newLayer = std::make_unique<renderLayer>(std::vector<sf::Drawable*>{drawable}, zOrder);
        renderQueue.push_back(std::move(newLayer));
    }

    sortRenderQueue();

    return true;
}

void RenderManager::sortRenderQueue()
{
    std::sort(renderQueue.begin(), renderQueue.end(), [](const std::unique_ptr<renderLayer>& a, const std::unique_ptr<renderLayer>& b)
    {
        return a->zOrder < b->zOrder;
    });
}

bool RenderManager::removeFromRenderQueue(sf::Drawable* drawable, ZOrder zOrder)
{
    assert(drawable != nullptr && "RenderManager::removeFromRenderQueue() drawable is nullptr");

    auto layersIt = std::find_if(renderQueue.begin(), renderQueue.end(), [zOrder](const std::unique_ptr<renderLayer>& layer)
    {
        return layer->zOrder == zOrder;
    });

    if (layersIt != renderQueue.end())
    {
        auto drawableIt = FindDrawable(drawable, *layersIt);
        
        if (drawableIt != layersIt->get()->drawables.end())
        {
            layersIt->get()->drawables.erase(drawableIt);
            return true;
        }
        
        return false;
    }

    return false;
}

std::vector<sf::Drawable*>::iterator RenderManager::FindDrawable(sf::Drawable* drawable, const std::unique_ptr<renderLayer>& layer)
{
    auto drawableIt = std::find(layer->drawables.begin(), layer->drawables.end(), drawable);

    return drawableIt;
}


void RenderManager::clearRenderQueue()
{
    renderQueue.clear();
}