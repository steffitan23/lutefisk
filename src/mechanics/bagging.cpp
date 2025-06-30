#include "bagging.h"

BaggingArea::BaggingArea(const sf::FloatRect &bounds,
                         std::vector<std::shared_ptr<Item>> &submittables, const sf::Font &font)
    : SubmittableArea<Item>(bounds, submittables, std::string{"Bag"}, font) {};

void BaggingArea::submit()
{
    if (!m_curr_submit_index.has_value())
    {
        return;
    }

    const auto i = m_curr_submit_index.value();
    (*m_submittables[i]).bag();
    (*m_submittables[i]).set_invisible();
    m_submittables.erase(m_submittables.begin() + i);
}