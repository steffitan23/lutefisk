#pragma once
#include <SFML/Graphics.hpp>

class Scene
{
public:
    virtual ~Scene() = default;
    virtual void handle_event(const sf::Event &event, const sf::RenderWindow &window) = 0;
    virtual void update(sf::RenderWindow &window) = 0;
    virtual void render(sf::RenderWindow &window) = 0;

    virtual bool is_finished() const { return false; }
    virtual std::string next_scene_key() const { return ""; }
};
