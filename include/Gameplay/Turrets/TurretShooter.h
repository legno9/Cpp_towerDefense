#pragma once
#include <Gameplay/Turrets/TurretBase.h>

class TurretShooter: public TurretBase
{
    public:

        TurretShooter(const sf::Vector2f& position, const std::string& configPath);

        ~TurretShooter() override = default;

        void action() override;

};