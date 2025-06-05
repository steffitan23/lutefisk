#pragma once

#include <SFML/Graphics.hpp>
#include "items.h"

class Scanner : public sf::Drawable, public sf::Transformable
{
public:
    Scanner(const sf::Texture &texture);

    void update_animation();
    // sf::Vector2f get_pos() const;
    void scan(Item &item);
    // sf::FloatRect get_bounds() const;
    // sf::Vector2f get_center() const;
    bool contains(Item &item) const;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    const sf::Texture &m_texture;
    sf::Sprite m_sprite;
    int m_frame_index;
    sf::Clock m_clock;
    bool m_animating;
};