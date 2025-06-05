#include "scan.h"
#include "items.h"
#include <SFML/Graphics.hpp>
#include <iostream>

namespace
{
    constexpr int FRAME_WIDTH = 512;
    constexpr int FRAME_HEIGHT = 512;
    constexpr size_t TOTAL_FRAMES = 10;
    constexpr float FRAME_DURATION = 0.07f;
    const sf::Vector2f SCANNER_POS{975.f, 575.f};
    const sf::Vector2f SCANNER_SCALE{0.6f, 0.6f};
}

Scanner::Scanner(const sf::Texture &texture)
    : m_texture(texture), m_sprite(texture)
{
    m_sprite.setTextureRect(sf::IntRect{{0, 0}, {512, 512}});
    m_sprite.setScale(SCANNER_SCALE);
    m_sprite.setPosition(SCANNER_POS);
}
void Scanner::update_animation()
{
    if (!m_animating)
        return;

    if (m_clock.getElapsedTime().asSeconds() >= FRAME_DURATION)
    {
        m_frame_index++;

        if (m_frame_index >= TOTAL_FRAMES)
        {
            std::cout << "Animation done!\n";
            m_animating = false;
            m_frame_index = 0;
            return;
        }

        m_sprite.setTextureRect(sf::IntRect{{m_frame_index * FRAME_WIDTH, 0}, {FRAME_WIDTH, FRAME_HEIGHT}});
        std::cout << "Next frame at " << m_frame_index * FRAME_WIDTH << std::endl;

        m_clock.restart();
    }
}

void Scanner::scan(Item &item)
{
    if (m_animating)
        return;

    item.scan();
    m_frame_index = 0;
    m_clock.restart();
    m_animating = true;
}

bool Scanner::contains(Item &item) const
{
    sf::FloatRect bounds = m_sprite.getGlobalBounds();

    constexpr float margin = 40.f;

    bounds.position += sf::Vector2f{margin, margin};
    bounds.size -= sf::Vector2f{2.f * margin, 2.f * margin};

    return bounds.contains(item.get_center());
}

void Scanner::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_sprite, states);
}
