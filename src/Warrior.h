/**
 * @file Warrior.h
 * @brief Deklaracja klasy Warrior - wojownik walczący wręcz
 */

#pragma once

#include "PlayerBase.h"

/**
 * @brief Wojownik - klasa postaci walczącej wręcz
 *
 * Dziedziczy po PlayerBase i reprezentuje postać zoptymalizowaną
 * pod kąt walki w zwarciu. Posiada największą ilość HP spośród
 * wszystkich klas, kosztem niższej prędkości.
 *
 * Statystyki:
 * - HP: 150 (najwyższe)
 * - Prędkość: 150 px/s (najniższa)
 * - Obrażenia: 25 (najwyższe)
 * - Zasięg: bliski (40px hitbox wręcz)
 */
class Warrior : public PlayerBase
{
public:
    /**
     * @brief Konstruktor wojownika
     *
     * Statystyki są zdefiniowane wewnątrz klasy i nie mogą być zmienione.
     * @param startX    Startowa pozycja X w pikselach
     * @param startY    Startowa pozycja Y w pikselach
     * @param upKey     Klawisz ruchu w górę
     * @param downKey   Klawisz ruchu w dół
     * @param leftKey   Klawisz ruchu w lewo
     * @param rightKey  Klawisz ruchu w prawo
     * @param attackKey Klawisz ataku wręcz
     */
    Warrior(float startX, float startY,
            sf::Keyboard::Key upKey,
            sf::Keyboard::Key downKey,
            sf::Keyboard::Key leftKey,
            sf::Keyboard::Key rightKey,
            sf::Keyboard::Key attackKey);

    /**
     * @brief Rysuje wojownika i jego elementy graficzne
     *
     * Wywołuje drawBase() z klasy bazowej która rysuje
     * kształt, pasek HP i ewentualny hitbox ataku.
     * @param window Referencja do okna gry
     */
    void draw(sf::RenderWindow& window) override;
};