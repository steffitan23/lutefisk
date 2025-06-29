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
    sm.register_scene("day1_bob", std::make_unique<CounterScene>());
    sm.switch_to("day1_bob");

    // std::vector<std::unique_ptr<Item>> items;

    // DraggableArea<Item> draggable_area{sf::FloatRect{{0, 0}, {1710, 956}}, items};
    // BaggingArea bagging_area{sf::FloatRect{{1400, 512}, {512, 512}}, items, font};

    // DialogueArea dialogue_area{sf::FloatRect{{20, 400}, {600, 140}}, sf::FloatRect({20, 400}, {600, 140}), font};

    // auto d_tree = DialogueTree::parse_json_to_tree("test/bob_test.json");
    // dialogue_area.set_tree(std::make_unique<DialogueTree>(d_tree));
    // dialogue_area.set_text_to_node((dialogue_area.get_tree()).get_start());

    // Screen screen_display{sf::FloatRect{{900, 150}, {512, 512}}, font};
    // Scanner scanner{SCAN_BUDDY};

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

        // for (auto &item : items)
        // {
        //     if (scanner.contains(*item) && !(*item).get_scanned())
        //     {
        //         scanner.scan(*item);
        //         screen_display.add(*item);
        //         break;
        //     }
        // }
        sm.render(window);
        sm.update(window);
    }
}
