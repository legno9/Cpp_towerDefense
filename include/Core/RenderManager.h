#pragma once
#include <memory>
#include <vector>

namespace sf
{
    class Drawable;
    class RenderWindow;
} 

enum class ZOrder : std::int8_t
        {
            Underground = -1,
            Background = 0,
            Foreground = 1,
            SpatialUI = 2,
            UI = 3,
            UIpp = 4,
            NZOrders
        };

class RenderManager
{
    public:
    
        RenderManager(sf::RenderWindow* window);
        ~RenderManager();

        void render();
        void addToRenderQueue(sf::Drawable* drawable, ZOrder zOrder);
        void removeFromRenderQueue(sf::Drawable* drawable, ZOrder zOrder);
        void clearRenderQueue();

    private:

        void sortRenderQueue();
        
        sf::RenderWindow* m_window{ nullptr };
        
        struct renderLayer
        {
            std::vector <sf::Drawable*> drawables;
            ZOrder zOrder;

            renderLayer(std::vector<sf::Drawable*> drawableList, ZOrder order) : drawables(drawableList), zOrder(order) {}
        };
        
        std::vector<std::unique_ptr<renderLayer>> renderQueue{};
};