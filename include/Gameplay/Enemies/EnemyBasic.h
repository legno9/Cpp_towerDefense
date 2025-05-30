#pragma once

#include <Gameplay/Enemies/EnemyBase.h>
#include <string>               

class EnemyBasic : public EnemyBase
{
public:
    
    EnemyBasic(const sf::Vector2f& initialPosition, const std::string& configPath,
               RenderManager& renderManager, const std::vector<sf::Vector2f>& pathPoints);
    ~EnemyBasic() override = default;
};