#pragma once
#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp> 

class RenderManager;

class TurretBase: public GameObject
{
    public:
		~TurretBase() override = default;

		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

		void update(float deltaMilliseconds) override;
        void sell();
        void buy();
        virtual void action();

        bool isMaxLevel = false;

	protected:

        enum class TurretFlags: uint8_t
        {
            None = 0,
            Sellable = 1 << 1,
            Upgradeable = 1 << 2,
            AreaDamage = 1 << 3,
            SingleTargetDamage = 1 << 4,
            SlowOnHit = 1 << 5,
            StuntOnHit = 1 << 6,
            //...
        };

        struct TurretInfo
        {
            sf::Vector2f position{ .0f, .0f };
			sf::Texture* texture{ nullptr };

            float damage{ .0f };
            float areaDamage{ .0f };
            float areaDamageRange{ .0f };
            float actionRange{ .0f };
            float actionRate{ .0f };

            int8_t buyPrice{ 0 };
            int8_t sellPrice{ 0 };
            int8_t upgradePrice{ 0 };

            TurretFlags flags{ TurretFlags::None };
        };

		sf::Sprite m_sprite;
        float m_damage{ .0f };
        float m_areaDamage{ .0f };
        float m_actionRange{ .0f };
        float m_actionRate{ .0f };

        int8_t m_buyPrice{ 0 };
        int8_t m_sellPrice{ 0 };
        int8_t m_upgradePrice{ 0 };

        TurretFlags m_flags{ TurretFlags::None };

        void CreateDefaultUpgradeInfo(TurretInfo* towerInfo) const; 

        friend TurretFlags operator|(TurretFlags a, TurretFlags b)
        {
            return static_cast<TurretFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
        }

        friend TurretFlags operator&(TurretFlags a, TurretFlags b)
        {
            return static_cast<TurretFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
        }

        friend TurretFlags operator~(TurretFlags a)
        {
            return static_cast<TurretFlags>(~static_cast<uint8_t>(a));
        }

    public:

        bool init(const TurretInfo& TurretInfo, RenderManager* renderManager);
        void upgrade(const TurretInfo &TurretInfo);

    private:

        RenderManager* m_renderManager{ nullptr };

        float m_actionTimer{ .0f };
        int8_t m_level{ 1 };
        int8_t m_maxLevel{ 3 };   

        float defaultMultiplier{ 1.5f }; 

};