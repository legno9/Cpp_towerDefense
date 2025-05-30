
#include <cstdio>
#include <SFML/System/Clock.hpp>
#include <Core/GameManager.h>
#include <Utils/Common.h>


int main()
{
    GameManager game;
    const bool gameInitialized = game.init("data/Config/gameConfig.json");

    if (gameInitialized == false)
    {
        printf("Game could not be initialized\n");
        return 0;
    }

    sf::Clock clock;

    while (game.isRunning())
    {
        const sf::Time elapsed = clock.restart();

        game.update(elapsed.asMilliseconds());
        game.render();
    }
}
