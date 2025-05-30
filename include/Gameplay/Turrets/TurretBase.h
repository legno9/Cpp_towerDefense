#pragma once

#include <string>
#include <memory>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp> 
#include <Utils/json.hpp>
#include <Gameplay/GameObject.h>
#include <Animation/AnimationComponent.h>

namespace sf
{
    class texture;
}

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

inline TurretFlags operator|=(TurretFlags& a, TurretFlags b)
{return a = static_cast<TurretFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));}

inline TurretFlags operator&(TurretFlags a, TurretFlags b)
{ return static_cast<TurretFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b)); }

inline TurretFlags operator~(TurretFlags a)
{ return static_cast<TurretFlags>(~static_cast<uint8_t>(a)); }

class TurretBase: public GameObject
{
    public:
        TurretBase(const sf::Vector2f& position, const std::string& configPath);
        ~TurretBase() override;

        void update(uint32_t deltaMilliseconds) override;

        void upgrade(const nlohmann::json& json);
        void sell();
        virtual void action();

        bool isMaxLevel() const { return m_level >= m_maxLevel; }

    protected:
        std::unique_ptr<sf::Sprite> m_sprite {nullptr};

        float m_damage{0.0f};
        float m_areaDamage{0.0f};
        float m_actionRange{0.0f};
        float m_actionRate{0.0f};

        int m_buyPrice{0};
        int m_sellPrice{0};
        int m_upgradePrice{0};

        TurretFlags m_flags{TurretFlags::None};

        float m_actionTimer{0.0f};
        int m_level{0};
        int m_maxLevel{0};

        std::unique_ptr<AnimationComponent> m_animationComponent {nullptr};

        void applyConfig(const nlohmann::json& json);

        const std::map<std::string, TurretFlags> flagMap 
        {
        {"Sellable", TurretFlags::Sellable},
        {"Upgradeable", TurretFlags::Upgradeable},
        {"AreaDamage", TurretFlags::AreaDamage},
        {"SingleTargetDamage", TurretFlags::SingleTargetDamage},
        {"SlowOnHit", TurretFlags::SlowOnHit},
        {"StuntOnHit", TurretFlags::StuntOnHit}
        };
};
