/**
 * @file Wall.cpp
 * @brief Implementacja klasy Wall
 */

#include "Wall.h"
#include <algorithm>

Wall::Wall(int x1, int y1, int x2, int y2,
           float cellSize,
           float offsetX,
           float offsetY)
    : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
{
    const float thickness = 8.f;

    // Obliczamy pozycję węzłów w pikselach uwzględniając offset siatki
    float px1 = offsetX + x1 * cellSize;
    float py1 = offsetY + y1 * cellSize;
    float px2 = offsetX + x2 * cellSize;
    float py2 = offsetY + y2 * cellSize;

    if (y1 == y2)
    {
        // Ściana pozioma - węzły mają takie samo Y
        m_shape.setSize(sf::Vector2f(cellSize, thickness));
        m_shape.setPosition(std::min(px1, px2), py1 - thickness / 2.f);
    }
    else
    {
        // Ściana pionowa - węzły mają takie samo X
        m_shape.setSize(sf::Vector2f(thickness, cellSize));
        m_shape.setPosition(px1 - thickness / 2.f, std::min(py1, py2));
    }

    m_shape.setFillColor(sf::Color(200, 180, 120)); // kolor drewna
    m_shape.setOutlineThickness(1.f);
    m_shape.setOutlineColor(sf::Color(150, 130, 80));
}

void Wall::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}

sf::FloatRect Wall::getBounds() const
{
    return m_shape.getGlobalBounds();
}

bool Wall::isSamePosition(int x1, int y1, int x2, int y2) const
{
    // Sprawdzamy w obu kierunkach bo A→B to samo co B→A
    return (m_x1 == x1 && m_y1 == y1 && m_x2 == x2 && m_y2 == y2) ||
           (m_x1 == x2 && m_y1 == y2 && m_x2 == x1 && m_y2 == y1);
}