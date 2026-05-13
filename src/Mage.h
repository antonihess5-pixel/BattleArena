#pragma once

#include "PlayerBase.h"

/**
 * @brief Mag - klasa pochodna od PlayerBase
 * 
 * Statystyki: małe HP (80), szybki (250), słabszy atak (15), duży zasięg (80px)
 * Mag ma unikalny efekt wizualny ataku - fioletowy pocisk
 */
class Mage : public PlayerBase
{
public:
    /**
     * @brief Konstruktor maga
     * Przyjmuje tylko pozycję i klawisze - statystyki są stałe dla tej klasy
     */
    Mage(float startX, float startY,
         sf::Keyboard::Key upKey,
         sf::Keyboard::Key downKey,
         sf::Keyboard::Key leftKey,
         sf::Keyboard::Key rightKey,
         sf::Keyboard::Key attackKey);

    // Mag nadpisuje draw żeby narysować fioletowy efekt ataku
    void draw(sf::RenderWindow& window) override;
};