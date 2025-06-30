#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class Area
{
public:
    Area(const sf::FloatRect &rect) : m_bounds(rect) {}
    virtual ~Area() = default;

    virtual void on_left_click(const sf::RenderWindow &window) = 0;
    virtual void on_left_release(const sf::RenderWindow &window) = 0;
    virtual void update(const sf::RenderWindow &window) = 0;

    bool contains(const sf::Vector2f &point) const
    {
        return m_bounds.contains(point);
    }

protected:
    sf::FloatRect m_bounds;
};

template <typename T>
class SubmittableArea : public Area
{
public:
    SubmittableArea(
        const sf::FloatRect &bounds,
        std::vector<std::shared_ptr<T>> &submittables,
        const std::string &action_text,
        const sf::Font &action_font)
        : Area(bounds),
          m_submittables(submittables),
          m_action_text(action_font)
    {
        m_action_text.setString(action_text);
        m_action_text.setCharacterSize(40);
        m_action_text.setFillColor(sf::Color::White);
    }

    void on_left_click(const sf::RenderWindow &window) override {};
    void on_left_release(const sf::RenderWindow &window) override {};

    void draw(sf::RenderTarget &window)
    {
        if (m_curr_submit_index.has_value())
        {
            const auto &item_pos = m_submittables[m_curr_submit_index.value()]->get_pos();
            m_action_text.setPosition(sf::Vector2f{item_pos.x + 10.f, item_pos.y - 40.f});
            window.draw(m_action_text);
        }
    }

    void update(const sf::RenderWindow &window) override
    {
        const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (contains(mousePos))
        {
            for (size_t i = 0; i < m_submittables.size(); ++i)
            {
                if (m_submittables[i]->get_bounds().contains(mousePos))
                {
                    m_curr_submit_index = i;
                    return;
                }
            }
        }

        m_curr_submit_index.reset();
    }

    virtual void submit() = 0;

    void set_submittables(std::vector<std::shared_ptr<T>> &submittables)
    {
        m_submittables = submittables;
    }

protected:
    sf::Text m_action_text;
    std::vector<std::shared_ptr<T>> &m_submittables;
    std::optional<size_t> m_curr_submit_index;
};

template <typename T>
class DraggableArea : public Area
{
public:
    DraggableArea(const sf::FloatRect &bounds, std::vector<std::shared_ptr<T>> &draggables)
        : Area(bounds), m_draggables(draggables) {}

    void on_left_click(const sf::RenderWindow &window) override
    {
        const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        for (size_t i = 0; i < m_draggables.size(); ++i)
        {
            auto &d = m_draggables[i];
            if (d->get_bounds().contains(mousePos))
            {
                m_curr_drag_index = i;
                m_offset = mousePos - d->get_pos();
                return;
            }
        }
    }

    void
    on_left_release(const sf::RenderWindow &window) override
    {
        m_curr_drag_index.reset();
        m_offset = sf::Vector2f(); // needed?
    }

    void update(const sf::RenderWindow &window) override
    {
        if (m_curr_drag_index.has_value())
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            auto &curr_drag = m_draggables[m_curr_drag_index.value()];

            sf::Vector2f newPos = mousePos - m_offset;
            sf::FloatRect itemBounds = curr_drag->get_bounds();

            float itemWidth = itemBounds.size.x;
            float itemHeight = itemBounds.size.y;

            newPos.x = std::clamp(
                newPos.x,
                m_bounds.position.x,
                m_bounds.position.x + m_bounds.size.x - itemWidth);

            newPos.y = std::clamp(
                newPos.y,
                m_bounds.position.y,
                m_bounds.position.y + m_bounds.size.y - itemHeight);

            curr_drag->set_pos(newPos);
        }
    }

    void set_draggables(std::vector<std::shared_ptr<T>> &draggables)
    {
        m_draggables = draggables;
    }

private:
    std::vector<std::shared_ptr<T>> &m_draggables;
    std::optional<size_t> m_curr_drag_index;
    sf::Vector2f m_offset;
};
