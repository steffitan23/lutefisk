#include <SFML/Graphics.hpp>
#include <iostream>
#include "mechanics/items.h"
#include "mechanics/scan.h"
#include "mechanics/areas.h"
#include "mechanics/screen.h"
#include "mechanics/bagging.h"
#include "dialogue/dialogue.h"

// TODO: Change to singleton
sf::Texture TOMATO_SOUP_CAN("assets/tomato_soup_can.png");
sf::Texture SCREEN("assets/screen.png");
sf::Texture BASKET("assets/basket.png");
sf::Texture BG("assets/background.png");
sf::Texture BOB("assets/bob.png");
sf::Texture TERMINAL("assets/terminal.png");
sf::Texture APPLETRON("assets/appletron.png");
sf::Texture SCAN_BUDDY("assets/scan_buddy.png");
sf::Font font{"assets/fonts/Chewy-Regular.ttf"};

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1710, 956}), "Groceries");
    window.setFramerateLimit(144);

    std::vector<std::unique_ptr<Item>> items;

    items.emplace_back(std::make_unique<Item>(TomatoSoupCan{TOMATO_SOUP_CAN}));
    items.emplace_back(std::make_unique<Item>(Appletron{APPLETRON}));

    DraggableArea<Item> draggable_area{sf::FloatRect{{0, 0}, {1710, 956}}, items};
    BaggingArea bagging_area{sf::FloatRect{{1400, 512}, {512, 512}}, items, font};

    DialogueArea dialogue_area{sf::FloatRect{{0, 0}, {512, 512}}, sf::FloatRect({20, 20}, {512, 300}), sf::FloatRect({400, 400}, {512, 400}), font};

    Screen screen_display{sf::FloatRect{{900, 150}, {512, 512}}, font};
    Scanner scanner{SCAN_BUDDY};

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                const auto &mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent->button == sf::Mouse::Button::Left)
                {
                    const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    if (draggable_area.contains(mousePos))
                        draggable_area.on_left_click(window);
                }
            }

            if (event->is<sf::Event::MouseButtonReleased>())
            {
                const auto &mouseEvent = event->getIf<sf::Event::MouseButtonReleased>();
                if (mouseEvent->button == sf::Mouse::Button::Left)
                {
                    const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    if (bagging_area.contains(mousePos))
                        bagging_area.submit();

                    if (draggable_area.contains(mousePos))
                        draggable_area.on_left_release(window);
                }
            }
        }

        for (auto &item : items)
        {
            if (scanner.contains(*item) && !(*item).get_scanned())
            {
                scanner.scan(*item);
                screen_display.add(*item);
                break;
            }
        }

        draggable_area.update(window);
        bagging_area.update(window);

        // dialogue_area.update(window);

        sf::Sprite screen{SCREEN};
        screen.setPosition(sf::Vector2f{760, 0});
        screen.setScale(sf::Vector2f{1.5, 1.5});

        sf::Sprite basket{BASKET};
        basket.setPosition(sf::Vector2f{24, 512});

        sf::Sprite bg{BG};
        bg.setPosition(sf::Vector2f{0, 0});

        sf::Sprite term{TERMINAL};
        term.setScale(sf::Vector2f{0.5f, 0.5f});

        term.setPosition(sf::Vector2f{1470, 200});

        sf::Sprite bob{BOB};
        bob.setScale(sf::Vector2f{0.8f, 0.8f});
        bob.setPosition(sf::Vector2f{70, 132});

        window.clear();
        window.draw(bg);
        window.draw(term);
        window.draw(bob);
        window.draw(screen);
        window.draw(basket);

        scanner.update_animation();

        window.draw(scanner);
        for (auto &item_ptr : items)
        {
            window.draw(*item_ptr);
        }

        bagging_area.draw(window);
        window.draw(screen_display);
        dialogue_area.draw(window);
        window.display();
    }
}
