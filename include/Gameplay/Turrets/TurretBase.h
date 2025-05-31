#pragma once

#include <string>
#include <memory>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp> 
#include <Utils/json.hpp>
#include <Gameplay/GameObject.h>
#include <Animation/AnimationComponent.h>
#include <Utils/Common.h>

namespace sf
{
    class texture;
}

class EnemyBase;

class TurretBase: public GameObject
{
    public:
        TurretBase(const sf::Vector2f& position, const std::string& configPath);
        ~TurretBase() override;

        void update(uint32_t deltaMilliseconds) override;

        void upgrade();
        void sell();
        void action(unsigned int currentTargetEnemyId);

        bool isMaxLevel() const { return m_level >= m_maxLevel; }

        virtual GameObjectType getType() const = 0;

    protected:
        std::unique_ptr<sf::Sprite> m_sprite {nullptr};

        float m_damage{0.0f};
        float m_actionRange{0.0f};
        float m_actionRate{0.0f};

        float m_actionTimer{0.0f};
        int m_level{1};
        int m_maxLevel{0};

        bool m_isAttacking{false};

        std::string m_projectileType {};
        std::string m_projectileConfigPath {};

        std::unique_ptr<AnimationComponent> m_animationComponent {nullptr};

        nlohmann::json m_allLevelsData {};
        nlohmann::json m_allAnimLevelsData{};

        void applyConfig(const nlohmann::json& json);
        void getAnimLevelsData(const nlohmann::json& json);
        void updateFacingDirection(unsigned int targetEnemyId);
        virtual void performAttack(unsigned int targetEnemyId);

        unsigned int getLeadEnemy();

};
