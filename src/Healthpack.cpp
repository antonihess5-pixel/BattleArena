/**
 * @file HealthPack.cpp
 * @brief Implementacja klasy HealthPack
 */

#include "HealthPack.h"

HealthPack::HealthPack(float x, float y, int healing)
    : m_active(true)
    , m_healing(healing)
{
    const float size = 30.f;

    // Tło apteczki - ciemnozielony kwadrat
    m_background.setSize(sf::Vector2f(size, size));
    m_background.setFillColor(sf::Color(0, 100, 0));
    m_background.setOutlineThickness(2.f);
    m_background.setOutlineColor(sf::Color(0, 200, 0));
    m_background.setPosition(x, y);

    // Pozioma belka krzyżyka
    m_horizontal.setSize(sf::Vector2f(size - 8.f, (size - 8.f) / 3.f));
    m_horizontal.setFillColor(sf::Color(0, 255, 0));
    m_horizontal.setPosition(
        x + 4.f,
        y + size / 2.f - (size - 8.f) / 6.f
    );

    // Pionowa belka krzyżyka
    m_vertical.setSize(sf::Vector2f((size - 8.f) / 3.f, size - 8.f));
    m_vertical.setFillColor(sf::Color(0, 255, 0));
    m_vertical.setPosition(
        x + size / 2.f - (size - 8.f) / 6.f,
        y + 4.f
    );
}

void HealthPack::draw(sf::RenderWindow& window) const
{
    if (!m_active) return;

    window.draw(m_background);
    window.draw(m_horizontal);
    window.draw(m_vertical);
}

sf::FloatRect HealthPack::getBounds() const
{
    return m_background.getGlobalBounds();
}

bool HealthPack::isActive() const { return m_active; }
void HealthPack::collect()        { m_active = false; }
int  HealthPack::getHealing() const { return m_healing; }