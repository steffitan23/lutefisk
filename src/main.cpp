#include <SFML/Graphics.hpp>
#include <iostream>
#include "mechanics/items.h"
#include "mechanics/scan.h"
#include "mechanics/areas.h"
#include "mechanics/screen.h"
#include "mechanics/bagging.h"
#include "dialogue/dialogue.h"
#include "scene/scene_manager.h"
#include "scene/counter_scene.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1710, 956}), "Groceries");
    window.setFramerateLimit(144);

    SceneManager sm;
    auto scene = std::make_unique<CounterScene>();
    scene->set_dialogue("test/bob_test.json"); // callback registered on correct instance
    sm.register_scene("day1_bob", std::move(scene));
    sm.switch_to("day1_bob");

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            sm.handle_event(event.value(), window);
        }

        sm.update(window);
        sm.render(window);
    }
}
