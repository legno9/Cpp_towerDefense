#pragma once

#include <Gameplay/Enemies/EnemyBase.h>
#include <string>               

class EnemyFast : public EnemyBase
{
public:
    
    EnemyFast(const sf::Vector2f& initialPosition, const std::string& configPath,
                const std::vector<sf::Vector2f>& pathPoints);
    ~EnemyFast() override = default;
};