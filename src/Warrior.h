#pragma once

#include "PlayerBase.h"

/**
 * @brief Wojownik - klasa pochodna od PlayerBase
 * 
 * Dziedziczenie oznacza że Warrior MA WSZYSTKO co PlayerBase
 * i dodatkowo może mieć swoje własne rzeczy.
 * 
 * Statystyki: duże HP (150), wolny (150), silny atak (25), bliski zasięg (40px)
 */
class Warrior : public PlayerBase
{
public:
    /**
     * @brief Konstruktor wojownika
     * Przyjmuje tylko pozycję i klawisze - statystyki są stałe dla tej klasy
     */
    Warrior(float startX, float startY,
            sf::Keyboard::Key upKey,
            sf::Keyboard::Key downKey,
            sf::Keyboard::Key leftKey,
            sf::Keyboard::Key rightKey,
            sf::Keyboard::Key attackKey);

    // Warrior nadpisuje draw żeby narysować swój unikalny wygląd
    void draw(sf::RenderWindow& window) override;
};