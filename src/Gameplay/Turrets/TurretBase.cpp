#include <Core/RenderManager.h>
#include <Gameplay/Turrets/TurretBase.h>

bool TurretBase::init(const TurretInfo &TurretInfo, RenderManager *renderManager)
{
    if (TurretInfo.texture == nullptr || renderManager == nullptr)
    {
        return false;
    }

    m_sprite.setTexture(*TurretInfo.texture);
    m_sprite.setPosition(TurretInfo.position);

    m_damage = TurretInfo.damage;
    m_areaDamage = TurretInfo.areaDamage;
    m_actionRange = TurretInfo.actionRange;
    m_actionRate = TurretInfo.actionRate;
    m_actionTimer = m_actionRate;
    m_buyPrice = TurretInfo.buyPrice;
    m_sellPrice = TurretInfo.sellPrice;
    m_upgradePrice = TurretInfo.upgradePrice;
    m_flags = TurretInfo.flags;

    m_renderManager = renderManager;

    buy();

    return true;
}

void TurretBase::update(float deltaMilliseconds)
{
    // printf("a %f ",m_sprite.getPosition().x);
    m_actionTimer -= deltaMilliseconds;
    if (m_actionTimer <= .0f)
    {
        action();
        m_actionTimer = m_actionRate;
    }
}

void TurretBase::action()
{

}

void TurretBase::buy()
{
    m_renderManager->addToRenderQueue(&m_sprite, ZOrder::Foreground);
}

void TurretBase::sell()
{
    m_renderManager->removeFromRenderQueue(&m_sprite, ZOrder::Foreground);
}

void TurretBase::upgrade (const TurretInfo &TurretInfo)
{
    if (m_level >= m_maxLevel || TurretInfo.texture == nullptr)
    {
        return;
    }

    m_sprite.setTexture(*TurretInfo.texture);
    m_damage = TurretInfo.damage;
    m_areaDamage = TurretInfo.areaDamage;
    m_actionRange = TurretInfo.actionRange;
    m_actionRate = TurretInfo.actionRate;
    m_actionTimer = m_actionRate;
    m_upgradePrice = TurretInfo.buyPrice;
    m_sellPrice = TurretInfo.sellPrice;

    m_level++;

    if (m_level >= m_maxLevel)
    {
        isMaxLevel = true;
        m_flags = m_flags & ~TurretFlags::Upgradeable;
    }
}


void TurretBase::CreateDefaultUpgradeInfo(TurretInfo *TurretInfo) const
{
    TurretInfo->areaDamage *= defaultMultiplier;
    TurretInfo->damage *= defaultMultiplier;
    TurretInfo->actionRate *= defaultMultiplier;
    TurretInfo->sellPrice *= defaultMultiplier;
    TurretInfo->upgradePrice += TurretInfo->upgradePrice;
}

