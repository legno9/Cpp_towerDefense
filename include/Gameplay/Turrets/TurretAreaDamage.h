#pragma once

#include <Gameplay/Turrets/TurretBase.h>

class TurretAreaDamage : public TurretBase
{
    public:
        TurretAreaDamage(const sf::Vector2f& position, const std::string& configPath);
        ~TurretAreaDamage() override = default;
};