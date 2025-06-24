#pragma once

#include <SFML/Graphics.hpp>

class ScannableItem;
class UnscannableItem;

class Item : public sf::Drawable, public sf::Transformable
{
public:
    Item(std::string name,
         sf::Vector2f position,
         sf::Vector2f size,
         const sf::Texture &texture);

    virtual ~Item() = default;
    std::string get_name() const;

    void set_pos(const sf::Vector2f &vec);
    void set_pos(float x, float y);
    sf::Vector2f get_pos() const;

    sf::FloatRect get_bounds() const;
    sf::Vector2f get_center() const;

    static std::unique_ptr<ScannableItem> create_scannable_item(const std::string &id, const sf::Texture &texture, float price);
    static std::unique_ptr<UnscannableItem> create_unscannable_item(const std::string &id, const sf::Texture &texture);

    virtual float get_price() const { return 0.0f; };
    virtual bool get_scanned() const { return false; };
    virtual void scan() {};
    virtual void bag() {};

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    const sf::Texture &m_texture;
    sf::Sprite m_sprite;
    std::string m_name;
};

class ScannableItem : public Item
{
public:
    ScannableItem(std::string name, sf::Vector2f position, sf::Vector2f size, const sf::Texture &texture, float price);
    float get_price() const override;
    bool get_scanned() const override;
    void scan() override;
    void bag() override;

private:
    float m_price;
    bool m_scanned;
    bool m_bagged;
};

class UnscannableItem : public Item
{
public:
    UnscannableItem(std::string name, sf::Vector2f position, sf::Vector2f size, const sf::Texture &texture)
        : Item(std::move(name), position, size, texture) {}
};