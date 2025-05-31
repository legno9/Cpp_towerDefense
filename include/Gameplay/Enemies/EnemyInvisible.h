#pragma once

#include <Gameplay/Enemies/EnemyBase.h>
#include <string>               

class EnemyInvisible : public EnemyBase
{
    public:
        
        EnemyInvisible(const sf::Vector2f& initialPosition, const std::string& configPath,
                    const std::vector<sf::Vector2f>& pathPoints);
        ~EnemyInvisible() override = default;

        void receiveDamage(float damage) override;

        bool isTargetable() const override { return !m_isInvisible; } 

    protected:
        
        bool m_isInvisible { true };
        std::string m_invisibilityRevealedPath {};

};