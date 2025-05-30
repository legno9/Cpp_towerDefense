#pragma once

#include <cstdint>

enum class GameObjectType: uint8_t
{
    None,
    BasicEnemy,
	FastEnemy,
	InvisibleEnemy,
    ShooterTurret,
	AreaDamageTurret,
	SlownessTurret,
};

enum class Direction : uint8_t 
{
    South,
    North,          
    East,         
    West,         
    None
};