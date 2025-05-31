#pragma once
#include <Gameplay/Turrets/TurretBase.h>

class TurretSlowness: public TurretBase
{
    public:

        TurretSlowness(const sf::Vector2f& position, const std::string& configPath);
        ~TurretSlowness() override = default;

};