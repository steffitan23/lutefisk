#pragma once

#include <SFML/Graphics.hpp>
#include "items.h"

class Screen : public sf::Drawable
{
public:
    Screen(const sf::FloatRect &bounds, const sf::Font &font);
    void add(const Item &item);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    const sf::FloatRect &m_bounds;
    std::vector<sf::Text> m_texts;
    const sf::Font &m_font;
    float m_offset;
};