#pragma once

#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp> 
#include <SFML/Graphics/Texture.hpp>
#include <Gameplay/GameObject.h>

class RenderManager;

enum class TurretFlags : uint8_t
{
    None = 0,
    Sellable = 1 << 1,
    Upgradeable = 1 << 2,
    AreaDamage = 1 << 3,
    SingleTargetDamage = 1 << 4,
    SlowOnHit = 1 << 5,
    StuntOnHit = 1 << 6,
};

inline TurretFlags operator|(TurretFlags a, TurretFlags b)
{ return static_cast<TurretFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b)); }

inline TurretFlags operator&(TurretFlags a, TurretFlags b)
{ return static_cast<TurretFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b)); }

inline TurretFlags operator~(TurretFlags a)
{ return static_cast<TurretFlags>(~static_cast<uint8_t>(a)); }

struct TurretConfig
{
    std::string texturePath;
    float damage;
    float areaDamage;
    float areaDamageRange;
    float actionRange;
    float actionRate;
    int buyPrice;
    int sellPrice;
    int upgradePrice;
    TurretFlags flags;
    int maxLevel;
};

class TurretBase: public GameObject
{
    public:
        TurretBase(const sf::Vector2f& position, const std::string& texturePath, const TurretConfig& config, RenderManager& renderManager);
        ~TurretBase() override = default;

		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

		void update(float deltaMilliseconds) override;

        void upgrade(const TurretConfig& newLevelConfig);
        virtual void action();

        bool isMaxLevel() const { return m_level >= m_maxLevel; }

	protected:

        sf::Sprite m_sprite;
        sf::Texture* m_currentTexture = nullptr;

        float m_damage;
        float m_areaDamage;
        float m_actionRange;
        float m_actionRate;

        int m_buyPrice;
        int m_sellPrice;
        int m_upgradePrice;

        TurretFlags m_flags;

        float m_actionTimer;
        int m_level;
        int m_maxLevel;

        RenderManager& m_renderManager;

        void applyConfig(const TurretConfig& config);

};