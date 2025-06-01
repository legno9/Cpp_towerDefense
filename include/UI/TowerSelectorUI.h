#pragma once

#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>
#include <Utils/Common.h>

class Button;
class TextLabel;
class GameManager; 

class TowerSelectorUI : public sf::Drawable, public sf::Transformable
{
public:

    TowerSelectorUI(GameManager& gameManager, const sf::Vector2f& position, const sf::Vector2f& size);
    ~TowerSelectorUI() override;

    void update(uint32_t ddeltaMilisecondseltaTime, const sf::Vector2f mousePos);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    void initializeSelector(const std::unordered_map<GameObjectType, std::string>& buildableTowers);

    GameObjectType getSelectedTowerType() const { return m_selectedTowerType; }
    const std::string& getSelectedTowerConfigPath() const { return m_selectedTowerConfigPath; }
    void deselectTower();

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:

    GameManager& m_gameManager;

    sf::RectangleShape m_backgroundPanel {};

    std::vector<std::unique_ptr<Button>> m_towerButtons {};

    std::unique_ptr<Button> m_sellButton {nullptr};
    std::unique_ptr<Button> m_upgradeButton {nullptr};

    GameObjectType m_selectedTowerType {GameObjectType::None};
    std::string m_selectedTowerConfigPath {};

    std::map<GameObjectType, std::string> m_buildableTowerConfigPaths {};

    sf::Vector2f m_position {};
    sf::Vector2f m_size {};

    bool m_upgradeButtonEnabled {false};
    bool m_sellButtonEnabled {false};

    void setupTowerButton(GameObjectType type, const sf::Vector2f& position, float size, const sf::Texture& iconTexture);
    void onUpgradeButtonClicked();
    void onSellButtonClicked();
};