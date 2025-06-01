#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <Core/AssetManager.h>

class TextLabel : public sf::Drawable, public sf::Transformable
{
public:

    TextLabel(const sf::Vector2f& position, const std::string& text, const sf::Color& color, unsigned int characterSize, sf::Font &font = AssetManager::getInstance().getDefaultFont());/////////
    ~TextLabel() override;

    void setText(const std::string& newText);
    const std::string& getText() const { return m_text.getString(); }

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Text m_text;
    sf::Font& m_font;
};