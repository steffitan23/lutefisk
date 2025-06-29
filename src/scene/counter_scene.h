#pragma once
#include <SFML/Graphics.hpp>
#include "scene.h"
#include "../mechanics/items.h"
#include "../dialogue/dialogue.h"
#include "../mechanics/areas.h"
#include "../mechanics/bagging.h"
#include "../mechanics/scan.h"
#include "../assets/asset_manager.h"

// TODO: Change to singleton
std::string TOMATO_SOUP_CAN("assets/tomato_soup_can.png");
std::string SCREEN("assets/screen.png");
std::string BASKET("assets/basket.png");
std::string BG("assets/background.png");
std::string BOB("assets/bob.png");
std::string TERMINAL("assets/terminal.png");
std::string APPLETRON("assets/appletron.png");
std::string SCAN_BUDDY("assets/scan_buddy.png");
std::string FONT{"assets/fonts/Chewy-Regular.ttf"};

std::vector<std::unique_ptr<Item>> starting_items = {};

class CounterScene : public Scene
{
public:
    CounterScene()
        : m_draggable_area{sf::FloatRect{{0, 0}, {1710, 956}},
                           starting_items},
          m_bagging_area{sf::FloatRect{{1400, 512}, {512, 512}},
                         starting_items, AssetManager::get().get_font(FONT)},
          m_dialogue_area{sf::FloatRect{{20, 400}, {600, 140}},
                          sf::FloatRect({20, 400}, {600, 140}),
                          AssetManager::get().get_font(FONT)},
          m_scanner{Scanner{AssetManager::get().get_texture(SCAN_BUDDY)}}
    {
    }

    void
    handle_event(const sf::Event &event, const sf::RenderWindow &window) override
    {
        if (event.is<sf::Event::MouseButtonPressed>())
        {
            const auto &mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouseEvent->button == sf::Mouse::Button::Left)
            {
                const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (m_draggable_area.contains(mousePos))
                    m_draggable_area.on_left_click(window);

                // if (m_dialogue_area.contains(mousePos))
                //     std::cout << "dia" << std::endl;

                // m_dialogue_area.on_left_click(window);
            }
        }

        if (event.is<sf::Event::MouseButtonReleased>())
        {
            const auto &mouseEvent = event.getIf<sf::Event::MouseButtonReleased>();
            if (mouseEvent->button == sf::Mouse::Button::Left)
            {
                const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (m_bagging_area.contains(mousePos))
                    m_bagging_area.submit();

                if (m_draggable_area.contains(mousePos))
                    m_draggable_area.on_left_release(window);
            }
        }
    }

    void update(sf::RenderWindow &window) override
    {
        m_draggable_area.update(window);
        m_bagging_area.update(window);
        m_dialogue_area.update(window);
        m_scanner.update_animation();
    }

    void render(sf::RenderWindow &window) override
    {
        sf::Sprite screen{AssetManager::get().get_texture(SCREEN)};
        screen.setPosition(sf::Vector2f{760, 0});
        screen.setScale(sf::Vector2f{1.5, 1.5});

        sf::Sprite basket{AssetManager::get().get_texture(BASKET)};
        basket.setPosition(sf::Vector2f{24, 512});

        sf::Sprite bg{AssetManager::get().get_texture(BG)};
        bg.setPosition(sf::Vector2f{0, 0});

        sf::Sprite term{AssetManager::get().get_texture(TERMINAL)};
        term.setScale(sf::Vector2f{0.5f, 0.5f});

        term.setPosition(sf::Vector2f{1470, 200});

        sf::Sprite bob{AssetManager::get().get_texture(BOB)};
        bob.setScale(sf::Vector2f{0.8f, 0.8f});
        bob.setPosition(sf::Vector2f{70, 20});

        window.clear();
        window.draw(bg);
        window.draw(term);
        window.draw(bob);
        window.draw(screen);
        window.draw(basket);

        window.draw(m_scanner);
        // for (auto &item_ptr : items)
        // {
        //     window.draw(*item_ptr);
        // }

        m_bagging_area.draw(window);
        // window.draw(screen_display);
        m_dialogue_area.draw(window);
        window.display();
    }

private:
    DraggableArea<Item> m_draggable_area;
    DialogueArea m_dialogue_area;
    BaggingArea m_bagging_area;
    Scanner m_scanner;
};