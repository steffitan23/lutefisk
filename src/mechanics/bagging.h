#pragma once

#include <SFML/Graphics.hpp>
#include "areas.h"
#include "items.h"

class BaggingArea : public SubmittableArea<Item>
{
public:
    BaggingArea(const sf::FloatRect &bounds,
                std::vector<std::shared_ptr<Item>> &submittables, const sf::Font &font);
    void submit() override;
};