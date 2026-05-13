#include "Warrior.h"

/**
 * Konstruktor wojownika - wywołuje konstruktor klasy bazowej
 * z ustalonymi statystykami wojownika.
 * 
 * "PlayerBase(...)" w liście inicjalizacyjnej to wywołanie
 * konstruktora klasy bazowej - tak przekazujemy dane "w górę" hierarchii
 */
Warrior::Warrior(float startX, float startY,
                 sf::Keyboard::Key upKey,
                 sf::Keyboard::Key downKey,
                 sf::Keyboard::Key leftKey,
                 sf::Keyboard::Key rightKey,
                 sf::Keyboard::Key attackKey)
    : PlayerBase(startX, startY,
                 sf::Color(50, 100, 255),  // niebieski
                 upKey, downKey, leftKey, rightKey, attackKey,
                 150,    // HP
                 150.f,  // prędkość
                 25,     // obrażenia
                 40.f)   // zasięg ataku
{
    // Wojownik jest większy niż bazowy gracz
    m_shape.setSize(sf::Vector2f(45.f, 45.f));

    // Obramowanie żeby wyglądał solidniej
    m_shape.setOutlineThickness(3.f);
    m_shape.setOutlineColor(sf::Color(200, 200, 255));
}

void Warrior::draw(sf::RenderWindow& window)
{
    // Wywołujemy rysowanie klasy bazowej (prostokąt, HP bar, atak)
    PlayerBase::draw(window);

    // Wojownik nie ma na razie dodatkowych efektów wizualnych
    // ale możemy tu dodać np. efekt "wściekłości" gdy HP spada
}