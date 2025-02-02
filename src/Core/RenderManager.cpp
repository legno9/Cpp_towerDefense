
#include <cassert>
#include <Core/RenderManager.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>


RenderManager::RenderManager(sf::RenderWindow *window)
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

void RenderManager::addToRenderQueue(sf::Drawable* drawable, ZOrder zOrder)
{
    assert(drawable != nullptr && "RenderManager::addToRender() drawable is nullptr");

    auto it = std::find_if(renderQueue.begin(), renderQueue.end(), [zOrder](const std::unique_ptr<renderLayer>& layer)
    {
        return layer->zOrder == zOrder;
    });

    if (it != renderQueue.end())
    {
        it->get()->drawables.push_back(drawable);
    }
    else
    {
        auto newLayer = std::make_unique<renderLayer>(std::vector<sf::Drawable*>{drawable}, zOrder);
        renderQueue.push_back(std::move(newLayer));
    }

    sortRenderQueue();
}

void RenderManager::sortRenderQueue()
{
    std::sort(renderQueue.begin(), renderQueue.end(), [](const std::unique_ptr<renderLayer>& a, const std::unique_ptr<renderLayer>& b)
    {
        return a->zOrder < b->zOrder;
    });
}

void RenderManager::removeFromRenderQueue(sf::Drawable* drawable, ZOrder zOrder)
{
    assert(drawable != nullptr && "RenderManager::removeFromRenderQueue() drawable is nullptr");

    auto it = std::find_if(renderQueue.begin(), renderQueue.end(), [zOrder](const std::unique_ptr<renderLayer>& layer)
    {
        return layer->zOrder == zOrder;
    });

    if (it != renderQueue.end())
    {
        auto drawableIt = std::find(it->get()->drawables.begin(), it->get()->drawables.end(), drawable);
        if (drawableIt != it->get()->drawables.end())
        {
            it->get()->drawables.erase(drawableIt);
        }
    }
}

void RenderManager::clearRenderQueue()
{
    renderQueue.clear();
}