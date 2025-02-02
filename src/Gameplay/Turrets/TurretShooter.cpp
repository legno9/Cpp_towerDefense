#include <Gameplay/Turrets/TurretShooter.h>
#include <Core/AssetManager.h>


TurretShooter::TurretShooter(RenderManager* renderManager)
{
    m_shooterTowerInfo.position = { 200.f, 200.f };
    m_shooterTowerInfo.texture = AssetManager::getInstance()->loadTexture("./Data/Turrets/tower_ninja_basic_0-0.png");
    m_shooterTowerInfo.damage = 10.f;
    m_shooterTowerInfo.areaDamage = 0.f;
    m_shooterTowerInfo.areaDamageRange = 0.f;
    m_shooterTowerInfo.actionRange = 100.f;
    m_shooterTowerInfo.actionRate = 1000.f;
    m_shooterTowerInfo.buyPrice = 10;
    m_shooterTowerInfo.sellPrice = 3;
    m_shooterTowerInfo.upgradePrice = 5;
    m_shooterTowerInfo.flags = TurretBase::TurretFlags::Sellable | TurretBase::TurretFlags::Upgradeable | TurretBase::TurretFlags::SingleTargetDamage;

    TurretBase::init(m_shooterTowerInfo, renderManager);
}