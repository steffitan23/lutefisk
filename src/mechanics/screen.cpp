#include "screen.h"
#include <sstream>
#include <iomanip>

constexpr float PADDING = 8.f;
constexpr float LINE_HEIGHT = 40.f;

Screen::Screen(const sf::FloatRect &bounds, const sf::Font &font) : m_bounds{bounds}, m_font{font}, m_offset{0.0f} {};

void Screen::add(const Item &item)
{
    sf::Text new_text{m_font};
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << item.get_price();
    new_text.setString(item.get_name() + std::string{"  $"} + oss.str());
    new_text.setPosition(sf::Vector2f{m_bounds.position.x + PADDING, m_bounds.position.y + PADDING + m_offset});
    new_text.setFillColor(sf::Color::Black);
    m_offset += LINE_HEIGHT;
    m_texts.emplace_back(new_text);
}

void Screen::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (const auto &text : m_texts)
    {
        target.draw(text, states);
    }
}