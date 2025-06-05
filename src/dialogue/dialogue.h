#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include "../mechanics/areas.h"

class DialogueArea : public Area
{
public:
    DialogueArea(sf::FloatRect bounds, sf::FloatRect text_area, sf::FloatRect options_area, sf::Font &font);
    void on_left_click(sf::RenderWindow &window) override;
    void on_left_release(sf::RenderWindow &window) override;
    void update(sf::RenderWindow &window) override;
    void draw(sf::RenderTarget &window);
    std::string wrapText(const std::string &input, unsigned int charSize, float maxWidth);

private:
    sf::FloatRect m_text_area;
    sf::FloatRect m_options_area;
    sf::Font &m_font;
};

class DialogueTree
{
public:
    DialogueTree(std::unique_ptr<Node> start);
    Node &get_start();

    template <typename T, typename... Args>
    DialogueTree make_tree(Args &&...args)
    {
        return DialogueTree(std::make_unique<T>(std::forward<Args>(args)...));
    };

private:
    std::unique_ptr<Node> m_start;
    Node &m_current;
};

class Node
{
public:
    Node(std::string text, std::string name);
    virtual std::string get_text() = 0;
    virtual std::string get_name() = 0;
    virtual Node &get_next();

protected:
    std::string m_text;
    std::string m_name;
    std::unique_ptr<Node> m_next;
};

class ExpositionNode : public Node
{
public:
    std::string get_text() override;
    Node &get_next() override;
};

class SpeechNode : public Node
{
public:
    std::string get_text() override;
    Node &get_next() override;
};

template <size_t N>
class OptionsNode : public Node
{
public:
    OptionsNode(std::string text,
                const std::array<std::string, N> &options_texts,
                std::array<std::unique_ptr<Node>, N> options_ptrs);

    std::string get_text() override
    {
        std::stringstream ss;
        ss << m_text << "\n";

        for (size_t i = 0; i < m_options_texts.size(); ++i)
        {
            ss << m_options_texts[i] << "\n";
        }

        return ss.str();
    }

    void select_option(size_t i);
    Node &get_next() override;

private:
    std::array<bool, N> m_visited;
    std::array<std::string, N> m_options_texts;
    std::array<std::unique_ptr<Node>, N> m_options_ptrs;
};