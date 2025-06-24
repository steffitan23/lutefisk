#include "items.h"

Item::Item(std::string name,
           sf::Vector2f position,
           sf::Vector2f size,
           const sf::Texture &texture) : m_name{name},
                                         m_texture{texture},
                                         m_sprite{texture}

{
    m_sprite.setPosition(position);
    const auto bounds = m_sprite.getLocalBounds();
    if (bounds.size.x > 0 && bounds.size.y > 0)
        m_sprite.setScale(sf::Vector2f{size.x / bounds.size.x, size.y / bounds.size.y});
}

ScannableItem::ScannableItem(std::string name,
                             sf::Vector2f position,
                             sf::Vector2f size,
                             const sf::Texture &texture,
                             float price)
    : Item::Item(std::move(name), position, size, texture), m_price{price}, m_scanned{false}, m_bagged{false} {}

std::unique_ptr<ScannableItem> Item::create_scannable_item(const std::string &id, const sf::Texture &tex, float price)
{
    if (id == "tomato_soup")
    {
        return std::make_unique<ScannableItem>(ScannableItem{std::string{"Tomato Soup"}, {160.f, 600.f}, {128.f, 256.f}, tex, 2.5f});
    }
    else if (id == "appletron")
    {
        return std::make_unique<ScannableItem>(ScannableItem{std::string{"Appletron"}, {180.f, 600.f}, {256.f, 256.f}, tex, 17.0f});
    }

    throw std::invalid_argument("Unknown item type: " + id);
}

// TODO: make unscannable items

std::string Item::get_name() const { return m_name; }

void Item::set_pos(const sf::Vector2f &vec) { m_sprite.setPosition(vec); }
void Item::set_pos(float x, float y) { set_pos(sf::Vector2f{x, y}); }

sf::Vector2f Item::get_pos() const { return m_sprite.getPosition(); }

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

float ScannableItem::get_price() const { return m_price; }

bool ScannableItem::get_scanned() const { return m_scanned; }

void ScannableItem::scan()
{
    m_scanned = true;
} // TODO: change.... maybe make private?

void ScannableItem::bag()
{
    m_bagged = true;
} // TODO: change.... maybe make private?