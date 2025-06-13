#include "dialogue.h"

float PADDING = 10;
float LINE_HEIGHT = 30;

DialogueArea::DialogueArea(sf::FloatRect bounds, sf::FloatRect text_area, sf::Font &font)
    : Area(bounds), m_text_area{text_area}, m_font{font}, m_text_obj{sf::Text{font}}, m_title_obj{sf::Text{font}} {};

void DialogueArea::set_text_to_node(Node &node)
{
    m_text_obj = sf::Text{m_font};
    m_title_obj = sf::Text{m_font};
    m_options_objs.clear();

    const auto &title_str = node.get_title();
    const auto &text_str = node.get_text();

    float x{m_text_area.position.x + PADDING};
    float y{m_text_area.position.y + PADDING};

    if (title_str != "")
    {
        m_title_obj.setFont(m_font);
        m_title_obj.setCharacterSize(30);
        m_title_obj.setFillColor(sf::Color::Black);
        m_title_obj.setString(wrapText(title_str, 24, m_text_area.size.x));
        m_title_obj.setPosition(sf::Vector2f{x, y});
        y = y + m_title_obj.getGlobalBounds().size.y + PADDING;
    }

    if (text_str != "")
    {
        m_text_obj.setFont(m_font);
        m_text_obj.setCharacterSize(24);
        m_text_obj.setFillColor(sf::Color::Black);
        m_text_obj.setString(wrapText(text_str, 24, m_text_area.size.x - 2 * PADDING));
        m_text_obj.setPosition(sf::Vector2f{x, y});
        y = y + m_text_obj.getGlobalBounds().size.y + PADDING;
    }

    if (node.get_node_type() == NodeType::Options)
    {
        const auto options_node = dynamic_cast<OptionsNode *>(&node);
        const auto options = options_node->get_options();
        const auto visited = options_node->get_visited();

        for (size_t i = 0; i < options.size(); ++i)
        {
            auto new_obj = sf::Text{m_font};
            new_obj.setCharacterSize(24);

            if (visited[i])
            {
                new_obj.setFillColor(sf::Color(128, 128, 128));
            }
            else
            {
                new_obj.setFillColor(sf::Color::Black);
            }

            new_obj.setString(wrapText(options[i], 24, m_text_area.size.x - 2 * PADDING));
            new_obj.setPosition(sf::Vector2f{x, y});
            m_options_objs.push_back(std::move(new_obj));
            y = y + LINE_HEIGHT;
        }
    }

    m_textbox.setSize(sf::Vector2f{m_text_area.size.x, m_text_area.size.y});
    m_textbox.setPosition(sf::Vector2f{m_text_area.position.x, m_text_area.position.y});
    m_textbox.setFillColor(sf::Color::White);
    m_textbox.setOutlineColor(sf::Color::Black);
    m_textbox.setOutlineThickness(3.f);
}

void DialogueArea::draw(sf::RenderTarget &window)
{
    window.draw(m_textbox);
    window.draw(m_title_obj);
    window.draw(m_text_obj);
    if (!m_options_objs.empty())
    {
        for (const auto &o : m_options_objs)
        {
            window.draw(o);
        }
    }
}

// TODO: maybe move to utils
std::string DialogueArea::wrapText(const std::string &input, unsigned int charSize, float maxWidth)
{
    std::istringstream iss(input);
    std::string word;
    std::string line, result;
    sf::Text text{m_font};
    text.setCharacterSize(charSize);

    while (iss >> word)
    {
        std::string testLine = line + (line.empty() ? "" : " ") + word;
        text.setString(testLine);

        if (text.getLocalBounds().size.x > maxWidth)
        {
            if (!line.empty())
            {
                result += line + "\n";
                line = word;
            }
            else
            {
                result += word + "\n";
                line.clear();
            }
        }
        else
        {
            line = testLine;
        }
    }

    if (!line.empty())
        result += line;

    return result;
}
void DialogueArea::on_left_click(sf::RenderWindow &window)
{
    const auto node_type = m_tree->get_curr_node_type();
    if (node_type == NodeType::Options)
    {
        const auto options_node = dynamic_cast<OptionsNode *>(&(m_tree->get_curr_node()));
        const auto options_visited = options_node->get_visited();
        const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (size_t i = 0; i < m_options_objs.size(); ++i)
        {
            if (m_options_objs[i].getGlobalBounds().contains(mousePos) && options_visited[i] == false)
            {
                m_tree->choose(i);
                break;
            }
        }
    }
    else if (node_type == NodeType::Exposition || node_type == NodeType::Speech)
    {
        m_tree->next();
    }

    set_text_to_node(m_tree->get_curr_node());
}

void DialogueArea::on_left_release(sf::RenderWindow &) {}
void DialogueArea::update(sf::RenderWindow &) {}

void DialogueArea::set_tree(std::unique_ptr<DialogueTree> tree)
{
    m_tree = std::move(tree);
}
