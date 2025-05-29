#include <Gameplay/Turrets/TurretShooter.h>
#include <SFML/System/Vector2.hpp>
#include <Core/AssetManager.h>
#include <Core/RenderManager.h>

TurretShooter::TurretShooter(const sf::Vector2f& position, const std::string& configPath, RenderManager& renderManager)
    : TurretBase(position, configPath, renderManager)
{

}

void TurretShooter::action()
{

}