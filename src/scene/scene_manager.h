#pragma once
#include <SFML/Graphics.hpp>
#include "scene.h"

class SceneManager
{
public:
    void register_scene(const std::string &key, std::unique_ptr<Scene> scene)
    {
        m_scenes[key] = std::move(scene);
    }

    void switch_to(const std::string &key)
    {
        if (m_scenes.count(key))
        {
            m_current = m_scenes[key].get();
        }
    }

    void handle_event(const sf::Event &event, const sf::RenderWindow &window)
    {
        if (m_current)
            m_current->handle_event(event, window);
    }

    void update(sf::RenderWindow &window)
    {
        if (m_current)
        {
            m_current->update(window);
            if (m_current->is_finished())
            {
                auto next = m_current->next_scene_key();
                if (!next.empty())
                    switch_to(next);
            }
        }
    }

    void render(sf::RenderWindow &window)
    {
        if (m_current)
            m_current->render(window);
    }

private:
    std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;
    Scene *m_current = nullptr;
};
