#pragma once

#include <functional>        
#include <string>            
#include <SFML/Graphics.hpp> 

class TextLabel;

class Button : public sf::Drawable, public sf::Transformable
{
public:

    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, unsigned int fontSize,
           const sf::Color& normalColor, const sf::Color& hoverColor, const sf::Color& clickedColor,
           const sf::Texture* iconTexture = nullptr);
    ~Button() override;

    void update(uint32_t deltaMiliseconds, const sf::Vector2f mousePos);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void setCallback(std::function<void()> callback) { m_callback = std::move(callback); }
    bool contains(const sf::Vector2f& mousePos) const;

protected:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::RectangleShape m_shape {};       
    sf::Sprite m_iconSprite {};    

    std::unique_ptr<TextLabel> m_text {nullptr};

    std::function<void()> m_callback;

    bool m_isHovered {false};
    bool m_isPressed {false}; 

    sf::Color m_normalColor {sf::Color(255, 255, 255, 255)};
    sf::Color m_hoverColor {sf::Color(255, 255, 255, 255)};
    sf::Color m_clickedColor {sf::Color(255, 255, 255, 255)};
};