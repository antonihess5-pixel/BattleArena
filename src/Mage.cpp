#include "Mage.h"

/**
 * Konstruktor maga - wywołuje konstruktor klasy bazowej
 * z ustalonymi statystykami maga.
 */
Mage::Mage(float startX, float startY,
           sf::Keyboard::Key upKey,
           sf::Keyboard::Key downKey,
           sf::Keyboard::Key leftKey,
           sf::Keyboard::Key rightKey,
           sf::Keyboard::Key attackKey)
    : PlayerBase(startX, startY,
                 sf::Color(180, 50, 255),  // fioletowy
                 upKey, downKey, leftKey, rightKey, attackKey,
                 80,     // HP - mało!
                 250.f,  // prędkość - szybki!
                 15,     // obrażenia - słabsze
                 80.f)   // zasięg - duży!
{
    // Mag jest mniejszy i smuklejszy
    m_shape.setSize(sf::Vector2f(30.f, 35.f));

    // Obramowanie w kolorze magii
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color(220, 150, 255));

    // Atak maga jest fioletowy zamiast żółtego
    m_attackShape.setFillColor(sf::Color(180, 50, 255, 180));
}

void Mage::draw(sf::RenderWindow& window)
{
    // Wywołujemy rysowanie klasy bazowej
    PlayerBase::draw(window);

    // Mag ma dodatkowy efekt - małe kółko "aury" wokół siebie
    if (m_attacking)
    {
        sf::CircleShape aura(25.f);
        aura.setFillColor(sf::Color(180, 50, 255, 40)); // bardzo przezroczysty
        aura.setPosition(
            m_shape.getPosition().x - 7.f,
            m_shape.getPosition().y - 7.f
        );
        window.draw(aura);
    }
}