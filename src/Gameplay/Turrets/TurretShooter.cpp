#include <Gameplay/Turrets/TurretShooter.h>
#include <SFML/System/Vector2.hpp>
#include <Core/AssetManager.h>
#include <Core/RenderManager.h>

// TurretShooter::TurretShooter(const sf::Vector2f& position, const std::string& texturePath, const TurretConfig& config,
//                          const std::string& arrowTexturePath, RenderManager& renderManager)
//     : TurretBase(position, texturePath, config, renderManager)
// {
    
// }

TurretShooter::TurretShooter(const sf::Vector2f& position, RenderManager& renderManager)
    : TurretBase(position, "./data/Turrets/tower_ninja_gun_0-0.png", createTemporaryTurretConfig(), renderManager)
{
    printf("TurretShooter::TurretShooter: Created a temporary turret shooter at position (%f, %f)\n",
          position.x, position.y);
}

TurretConfig TurretShooter::createTemporaryTurretConfig()
{
    TurretConfig config;
    config.texturePath = "./data/Turrets/tower_ninja_gun_0-0.png";
    config.damage = 10.0f;
    config.areaDamage = 0.0f;
    config.areaDamageRange = 0.0f;
    config.actionRange = 200.0f;
    config.actionRate = 1000.0f; // Asegúrate de que sea en milisegundos si delta es milisegundos
    config.buyPrice = 100;
    config.sellPrice = 50;
    config.upgradePrice = 150;
    config.flags = TurretFlags::None;
    config.maxLevel = 3;
    return config;
}

void TurretShooter::action()
{
    // 1. Lógica para encontrar un objetivo (enemigo) dentro del rango de acción.
    // Esto es complejo y requeriría acceso a la lista de enemigos del EntityManager.
    // Por simplicidad, asumimos que siempre encuentra un objetivo en (X, Y) o un objetivo de prueba.

    // sf::Vector2f targetPosition = findNearestEnemyInRango(m_position, m_actionRange); // <-- Esto sería un método real


}