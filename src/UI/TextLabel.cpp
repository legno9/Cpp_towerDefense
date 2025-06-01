#include <UI/TextLabel.h>
#include <Core/AssetManager.h>
#include <Core/RenderManager.h>
#include <iostream>

TextLabel::TextLabel(const sf::Vector2f& position, const std::string& text, const sf::Color& color, unsigned int characterSize, sf::Font &font)
    : m_font(font)
{
    m_text.setFont(m_font);
    m_text.setString(text);
    m_text.setCharacterSize(characterSize);
    m_text.setFillColor(color);
    m_text.setPosition(position);
    m_text.setOutlineColor(sf::Color::Black);
    m_text.setOutlineThickness(1.0f);

    RenderManager::getInstance().addToRenderQueue(*this, ZOrder::UI);
}

TextLabel::~TextLabel() 
{
    RenderManager::getInstance().removeFromRenderQueue(*this, ZOrder::UI);
}

void TextLabel::setText(const std::string& newText) 
{
    m_text.setString(newText);
}

void TextLabel::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    target.draw(m_text, states);
}