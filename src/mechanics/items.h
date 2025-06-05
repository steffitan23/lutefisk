#pragma once

#include <SFML/Graphics.hpp>

class Item : public sf::Drawable, public sf::Transformable
{
public:
    Item(std::string name, float price, const sf::Vector2f &pos, const sf::Vector2f &size, const sf::Texture &texture);

    std::string get_name() const;
    float get_price() const;
    void set_pos(const sf::Vector2f &vec);
    void set_pos(float x, float y);
    sf::Vector2f get_pos() const;
    bool get_scanned() const;
    void scan();
    void bag();
    sf::FloatRect get_bounds() const;
    sf::Vector2f get_center() const;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    float m_price;
    bool m_scanned;
    bool m_bagged;
    const sf::Texture &m_texture;
    sf::Sprite m_sprite;
    std::string m_name;
};

class TomatoSoupCan : public Item
{
public:
    TomatoSoupCan(const sf::Texture &tex);
};

class Appletron : public Item
{
public:
    Appletron(const sf::Texture &tex);
};
