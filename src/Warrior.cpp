/**
 * @file Warrior.cpp
 * @brief Implementacja klasy Warrior
 */

#include "Warrior.h"

Warrior::Warrior(float startX, float startY,
                 sf::Keyboard::Key upKey,
                 sf::Keyboard::Key downKey,
                 sf::Keyboard::Key leftKey,
                 sf::Keyboard::Key rightKey,
                 sf::Keyboard::Key attackKey)
    : PlayerBase(startX, startY,
                 sf::Color(50, 100, 255), // niebieski
                 upKey, downKey, leftKey, rightKey, attackKey,
                 150,   // HP - najwyższe spośród klas
                 150.f, // prędkość - najniższa spośród klas
                 25,    // obrażenia - najwyższe spośród klas
                 40.f)  // zasięg ataku wręcz
{
    // Wojownik jest większy od pozostałych postaci
    m_shape.setSize(sf::Vector2f(45.f, 45.f));

    // Grube obramowanie podkreśla masywność postaci
    m_shape.setOutlineThickness(3.f);
    m_shape.setOutlineColor(sf::Color(200, 200, 255));
}

void Warrior::draw(sf::RenderWindow& window)
{
    // Rysujemy wspólne elementy przez klasę bazową
    drawBase(window);
}