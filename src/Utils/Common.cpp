#include "Utils/Common.h"


static const std::unordered_map<std::string, GameObjectType> s_gameObjectTypeMap = 
{
    {"BasicEnemy", GameObjectType::BasicEnemy},
    {"FastEnemy", GameObjectType::FastEnemy},
    {"InvisibleEnemy", GameObjectType::InvisibleEnemy},
    {"ShooterTurret", GameObjectType::ShooterTurret},
    {"AreaDamageTurret", GameObjectType::AreaDamageTurret},
    {"SlownessTurret", GameObjectType::SlownessTurret},
    {"ShooterProjectile", GameObjectType::ShooterProjectile},
    {"AreaDamageProjectile", GameObjectType::AreaDamageProjectile},
    {"SlownessProjectile", GameObjectType::SlownessProjectile},
};


GameObjectType stringToGameObjectType(const std::string& typeStr)
{
    auto it = s_gameObjectTypeMap.find(typeStr);

    if (it != s_gameObjectTypeMap.end())
    {
        return it->second;
    }

    return GameObjectType::None;
}