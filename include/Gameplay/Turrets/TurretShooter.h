#pragma once
#include <Gameplay/Turrets/TurretBase.h>

class RenderManager;

class TurretShooter: public TurretBase
{
    public:

        TurretShooter(const sf::Vector2f& position, RenderManager& renderManager);

        ~TurretShooter() override = default;

        void action() override;

};