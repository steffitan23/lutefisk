#include "items.h"

Item::Item(std::string name, float price, const sf::Vector2f &pos, const sf::Vector2f &size, const sf::Texture &texture)
    : m_name{name}, m_price{price}, m_scanned{false}, m_bagged{false}, m_texture{texture}, m_sprite{texture}
{
    m_sprite.setPosition(pos);
    const auto bounds = m_sprite.getLocalBounds();
    if (bounds.size.x > 0 && bounds.size.y > 0)
        m_sprite.setScale(sf::Vector2f{size.x / bounds.size.x, size.y / bounds.size.y});
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

TomatoSoupCan::TomatoSoupCan(const sf::Texture &tex)
    : Item("Can of Tomato Soup", 2.5f, {160.f, 600.f}, {128.f, 256.f}, tex) {}

Appletron::Appletron(const sf::Texture &tex)
    : Item("APPLETRON 3000", 17.0f, {180.f, 600.f}, {256.f, 256.f}, tex) {}