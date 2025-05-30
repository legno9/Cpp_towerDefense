#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>

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

        static RenderManager& getInstance();
        static void initialize(sf::RenderWindow& window);
        
        void render();
        bool addToRenderQueue(sf::Drawable& drawable, ZOrder zOrder);
        bool removeFromRenderQueue(sf::Drawable& drawable, ZOrder zOrder);
        void clearRenderQueue();

    private:

        ~RenderManager();
        RenderManager(sf::RenderWindow& window);
        
        RenderManager(const RenderManager&) = delete;
        RenderManager& operator=(const RenderManager&) = delete;

        sf::RenderWindow& m_window;
        static RenderManager* s_instance;
        
        struct renderLayer
        {
            std::vector<std::reference_wrapper<sf::Drawable>> drawables {};
            ZOrder zOrder;

            renderLayer(sf::Drawable& drawable) : drawables{std::ref(drawable)} {}
        };
        
        std::map<ZOrder, std::unique_ptr<renderLayer>> m_renderLayers {};
};