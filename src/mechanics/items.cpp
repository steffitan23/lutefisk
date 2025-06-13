#include "items.h"

std::unique_ptr<Item> Item::create_item(const std::string &id, const sf::Texture &tex)
{
    if (id == "tomato_soup")
    {
        return std::make_unique<Item>(ItemConfig{"Tomato Soup", 2.5f, {160.f, 600.f}, {128.f, 256.f}, tex});
    }
    else if (id == "appletron")
    {
        return std::make_unique<Item>(ItemConfig{"Appletron", 17.0f, {180.f, 600.f}, {256.f, 256.f}, tex});
    }

    throw std::invalid_argument("Unknown item type: " + id);
}

Item::Item(const ItemConfig &config)
    : m_name{config.name},
      m_price{config.price},
      m_scanned{false},
      m_bagged{false},
      m_texture{config.texture},
      m_sprite{config.texture}

{
    m_sprite.setPosition(config.position);
    const auto bounds = m_sprite.getLocalBounds();
    if (bounds.size.x > 0 && bounds.size.y > 0)
        m_sprite.setScale(sf::Vector2f{config.size.x / bounds.size.x, config.size.y / bounds.size.y});
}

std::string Item::get_name() const { return m_name; }
float Item::get_price() const { return m_price; }

void Item::set_pos(const sf::Vector2f &vec) { m_sprite.setPosition(vec); }
void Item::set_pos(float x, float y) { set_pos(sf::Vector2f{x, y}); }

sf::Vector2f Item::get_pos() const { return m_sprite.getPosition(); }

bool Item::get_scanned() const { return m_scanned; }

void Item::scan()
{
    m_scanned = true;
} // TODO: change.... maybe make private?

void Item::bag()
{
    m_bagged = true;
} // TODO: change.... maybe make private?

sf::FloatRect Item::get_bounds() const { return m_sprite.getGlobalBounds(); }

sf::Vector2f Item::get_center() const
{
    return get_bounds().getCenter();
}

void Item::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_sprite, states);
}
