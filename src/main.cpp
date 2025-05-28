
#include <cstdio>
#include <Core/GameManager.h>
#include <SFML/System/Clock.hpp>
#include <Utils/Constants.h>

int main()
{
    // To-Do: Load game config from file instead of hardcoding values in code
    GameManager::GameCreateInfo gameCI;
    gameCI.gameTitle = GAME_TITLE;
    gameCI.screenWidth = 960u;
    gameCI.screenHeight = 540u;
    gameCI.frameRateLimit = 60u;

    GameManager game;
    const bool gameInitialized = game.init(gameCI);

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
