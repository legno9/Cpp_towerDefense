#pragma once
#include <Gameplay/Turrets/TurretBase.h>

class RenderManager;

class TurretShooter: public TurretBase
{
    public:

        TurretShooter(const sf::Vector2f& position, const std::string& configPath, RenderManager& renderManager);

        ~TurretShooter() override = default;

        void action() override;

};