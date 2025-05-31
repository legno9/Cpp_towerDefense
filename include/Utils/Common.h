#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

enum class GameObjectType: uint8_t
{
    None,
    BasicEnemy,
	FastEnemy,
	InvisibleEnemy,
    ShooterTurret,
	AreaDamageTurret,
	SlownessTurret,
    ShooterProjectile,
    AreaDamageProjectile,
    SlownessProjectile
};

enum class Direction : uint8_t 
{
    South,
    North,          
    East,         
    West,         
    None
};

GameObjectType stringToGameObjectType(const std::string& typeStr);