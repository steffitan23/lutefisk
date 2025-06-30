#pragma once

#include <SFML/Graphics.hpp>
#include "../assets/asset_manager.h"

class ScannableItem;
class UnscannableItem;

class Item : public sf::Drawable, public sf::Transformable
{
public:
    Item(std::string name,
         sf::Vector2f position,
         sf::Vector2f size,
         const sf::Texture &texture,
         bool can_drag,
         bool can_bag);

    virtual ~Item() = default;
    std::string get_name() const;

    void set_pos(const sf::Vector2f &vec);
    void set_pos(float x, float y);
    sf::Vector2f get_pos() const;

    sf::FloatRect get_bounds() const;
    sf::Vector2f get_center() const;

    void set_invisible();
    bool is_visible();

    static ScannableItem create_scannable_item(const std::string &id);
    static UnscannableItem create_unscannable_item(const std::string &id);

    virtual float get_price() const { return 0.0f; };
    virtual bool get_scanned() const { return false; };
    virtual void scan() {};
    virtual void bag() {};

    bool can_drag() { return m_can_drag; }
    bool can_bag() { return m_can_bag; }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    const sf::Texture &m_texture;
    sf::Sprite m_sprite;
    std::string m_name;
    bool m_visible;
    bool m_can_drag;
    bool m_can_bag;
};

class ScannableItem : public Item
{
public:
    ScannableItem(std::string name,
                  sf::Vector2f position,
                  sf::Vector2f size,
                  const sf::Texture &texture,
                  float price,
                  bool can_drag,
                  bool can_bag);
    float get_price() const override;
    bool get_scanned() const override;
    void scan() override;

private:
    float m_price;
    bool m_scanned;
};

class UnscannableItem : public Item
{
public:
    UnscannableItem(std::string name,
                    sf::Vector2f position,
                    sf::Vector2f size,
                    const sf::Texture &texture,
                    bool can_drag,
                    bool can_bag);
};