#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include <memory>
#include <vector>
#include <array>
#include "../mechanics/areas.h"
#include "nodes.h"

class DialogueTree
{
public:
    DialogueTree(std::shared_ptr<Node> start)
        : m_start(start), m_current(m_start.get()) {}

    Node &get_start() { return *m_start; }
    NodeType get_curr_node_type() { return m_current->get_node_type(); }
    void choose(size_t i)
    {
        m_current->select_option(i);
        next();
    }

    void set_add_item_callback(std::function<void(std::shared_ptr<Item>)> callback)
    {
        m_add_item_callback = std::move(callback);
    }

    void next()
    {
        if (m_current->has_next())
        {
            auto &next = m_current->get_next();
            if (next.get_node_type() == NodeType::Options)
            {
                auto options_node = dynamic_cast<OptionsNode *>(&(m_current->get_next()));
                if (options_node->all_visited())
                {
                    m_current = &(options_node->get_else());
                    return;
                }
            }

            if (next.get_node_type() == NodeType::Exposition)
            {
                auto expo_node = dynamic_cast<ExpositionNode *>(&(m_current->get_next()));
                if (expo_node->has_items())
                {
                    if (m_add_item_callback)
                    {
                        for (const auto &item : expo_node->get_items())
                        {
                            std::cout << "gotten items" << std::endl;
                            m_add_item_callback(item);
                        }
                    }
                }
            }
            m_current = &m_current->get_next();
        }
    }

    Node &get_curr_node() { return *m_current; }

    static DialogueTree parse_json_to_tree(const std::string &filename);

    template <typename T, typename... Args>
    static DialogueTree make_tree(Args &&...args)
    {
        return DialogueTree(std::make_unique<T>(std::forward<Args>(args)...));
    }

private:
    std::shared_ptr<Node> m_start;
    Node *m_current;
    std::function<void(std::shared_ptr<Item>)> m_add_item_callback;
};

class DialogueArea : public Area
{
public:
    DialogueArea(sf::FloatRect bounds, sf::FloatRect text_area, sf::Font &font);
    void on_left_click(const sf::RenderWindow &window) override;
    void on_left_release(const sf::RenderWindow &window) override;
    void update(const sf::RenderWindow &window) override;
    void draw(sf::RenderTarget &window);
    std::string wrapText(const std::string &input, unsigned int charSize, float maxWidth);

    void set_tree(std::unique_ptr<DialogueTree> tree);
    DialogueTree &get_tree() { return *m_tree; };
    void set_text_to_node(Node &node);
    void set_add_item_callback(std::function<void(std::shared_ptr<Item>)> callback);

private:
    sf::FloatRect m_text_area;
    sf::RectangleShape m_textbox;
    sf::Text m_title_obj;
    sf::Text m_text_obj;
    std::vector<sf::Text> m_options_objs;
    sf::Font &m_font;
    std::unique_ptr<DialogueTree> m_tree;
};