#pragma once
#include <Gameplay/Turrets/TurretBase.h>

class RenderManager;

class TurretShooter: public TurretBase
{
    public:

        TurretShooter(RenderManager* renderManager);
        ~TurretShooter() override = default;

    private:

        TurretBase::TurretInfo m_shooterTowerInfo;
};