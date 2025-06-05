#include "dialogue.h"

DialogueArea::DialogueArea(sf::FloatRect bounds, sf::FloatRect text_area, sf::FloatRect options_area, sf::Font &font)
    : Area(bounds), m_text_area{text_area}, m_options_area{options_area}, m_font{font} {};

void DialogueArea::draw(sf::RenderTarget &window)
{
    sf::RectangleShape textbox(sf::Vector2f{m_text_area.size.x, m_text_area.size.y});
    textbox.setPosition(sf::Vector2f{m_text_area.position.x, m_text_area.position.y});
    textbox.setFillColor(sf::Color::White);
    textbox.setOutlineColor(sf::Color::Black);
    textbox.setOutlineThickness(3.f);

    sf::Text text{m_font};
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setString(wrapText("what's up guydajdiadjaijdiadiadjajdi ajdaidjiadjiajdiajdiadjaidjsy", 24, 300));
    text.setPosition(sf::Vector2f{m_text_area.position.x + 10, m_text_area.position.y + 10});
    window.draw(textbox);
    window.draw(text);
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

void DialogueArea::on_left_click(sf::RenderWindow &window) {};
void DialogueArea::on_left_release(sf::RenderWindow &window) {};
void DialogueArea::update(sf::RenderWindow &window) {};