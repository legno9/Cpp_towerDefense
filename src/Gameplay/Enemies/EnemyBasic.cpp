#include <Gameplay/Enemies/EnemyBasic.h>                   

EnemyBasic::EnemyBasic(const sf::Vector2f& initialPosition, const std::string& configPath,
                       RenderManager& renderManager,const std::vector<sf::Vector2f>& pathPoints)

    : EnemyBase(initialPosition, configPath, renderManager, pathPoints)
{
    
}