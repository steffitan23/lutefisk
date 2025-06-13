#pragma once

#include <SFML/Graphics.hpp>

struct ItemConfig
{
    std::string name;
    float price;
    sf::Vector2f position;
    sf::Vector2f size;
    const sf::Texture &texture;
};

class Item : public sf::Drawable, public sf::Transformable
{
public:
    Item(const ItemConfig &config);

    virtual ~Item() = default;
    std::string get_name() const;
    float get_price() const;
    void set_pos(const sf::Vector2f &vec);
    void set_pos(float x, float y);
    sf::Vector2f get_pos() const;
    bool get_scanned() const;
    virtual void scan();
    void bag();
    sf::FloatRect get_bounds() const;
    sf::Vector2f get_center() const;
    static std::unique_ptr<Item> create_item(const std::string &id, const sf::Texture &texture);

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
