#include <UI/Button.h>
#include <iostream>
#include <algorithm>
#include <Core/AssetManager.h>
#include <Core/RenderManager.h>
#include <UI/TextLabel.h>

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, unsigned int fontSize,
               const sf::Color& normalColor, const sf::Color& hoverColor, const sf::Color& clickedColor,
               const sf::Texture* iconTexture)
    : m_normalColor(normalColor),
      m_hoverColor(hoverColor),
      m_clickedColor(clickedColor)
{
    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(m_normalColor);
    m_shape.setOutlineColor(sf::Color::Black);
    m_shape.setOutlineThickness(1.0f);

    m_text = std::make_unique<TextLabel>(sf::Vector2f(position.x + size.x / 2.0f, position.y + size.y / 2.0f), text, sf::Color::White, fontSize);

    if (iconTexture) 
    {
        m_iconSprite.setTexture(*iconTexture);

        float scaleX = size.x / m_iconSprite.getLocalBounds().width;
        float scaleY = size.y / m_iconSprite.getLocalBounds().height;
        m_iconSprite.setScale(scaleX, scaleY);

        m_iconSprite.setOrigin(m_iconSprite.getLocalBounds().width / 2, m_iconSprite.getLocalBounds().height / 2);
        m_iconSprite.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
    }

    RenderManager::getInstance().addToRenderQueue(*this, ZOrder::UI);
}

Button::~Button() 
{
    RenderManager::getInstance().removeFromRenderQueue(*this, ZOrder::UI);
    m_callback = nullptr;
}

void Button::update(uint32_t deltaMiliseconds, const sf::Vector2f mousePos) 
{
    bool wasHovered = m_isHovered;
    m_isHovered = contains(mousePos);

    if (m_isHovered && !wasHovered) 
    {
        if (!m_isPressed) m_shape.setFillColor(m_hoverColor);
    } 
    else if (!m_isHovered && wasHovered) 
    { 
        if (!m_isPressed) m_shape.setFillColor(m_normalColor);
    }
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) 
{
    sf::Vector2f mouseFloatPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) 
        {
            if (contains(mouseFloatPos)) {
                m_isPressed = true;
                m_shape.setFillColor(m_clickedColor);
            }
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) 
    {
        if (event.mouseButton.button == sf::Mouse::Left) 
        {
            if (m_isPressed) 
            {
                m_isPressed = false;
                if (contains(mouseFloatPos)) 
                { 
                    if (m_callback) 
                    {
                        m_callback();
                    }
                }
            }
            m_shape.setFillColor(m_isHovered ? m_hoverColor : m_normalColor);
        }
    }
}

bool Button::contains(const sf::Vector2f& mousePos) const 
{
    return m_shape.getGlobalBounds().contains(mousePos);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    target.draw(m_shape, states);
    if (m_iconSprite.getTexture()) 
    {
        target.draw(m_iconSprite, states);
    }
}