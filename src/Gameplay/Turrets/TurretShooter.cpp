#include <Gameplay/Turrets/TurretShooter.h>
#include <SFML/System/Vector2.hpp>
#include <Core/AssetManager.h>
#include <Core/RenderManager.h>

TurretShooter::TurretShooter(const sf::Vector2f& position, RenderManager& renderManager)
    : TurretBase(position, "./data/Config/Turrets/turretShooter.json", renderManager)
{

}

void TurretShooter::action()
{

}