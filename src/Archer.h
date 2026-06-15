/**
 * @file Archer.h
 * @brief Deklaracja klasy Archer - łucznik strzelający strzałami
 */

#pragma once

#include "PlayerBase.h"

/**
 * @brief Łucznik - klasa postaci strzelającej strzałami na dystans
 *
 * Dziedziczy po PlayerBase i nadpisuje metody update() oraz draw()
 * aby zaimplementować strzelanie strzałami. Stanowi zbalansowane
 * połączenie między Warriorem (HP, obrażenia) a Magem (prędkość, zasięg).
 *
 * Strzały łucznika różnią się od pocisków maga:
 * - Wolniejsze (280px/s vs 400px/s)
 * - Dłuższy cooldown (0.6s vs 0.3s)
 * - Wyższe obrażenia niż mag (20 vs 15)
 * - Inny wygląd (zielona kreska zamiast fioletowego kółka)
 *
 * Statystyki:
 * - HP: 110 (średnie)
 * - Prędkość: 200 px/s (średnia)
 * - Obrażenia: 20 (średnie)
 * - Zasięg: średni (strzały 280px/s, cooldown 0.6s)
 */
class Archer : public PlayerBase
{
public:
    /**
     * @brief Konstruktor łucznika
     *
     * Statystyki są zdefiniowane wewnątrz klasy i nie mogą być zmienione.
     * @param startX    Startowa pozycja X w pikselach
     * @param startY    Startowa pozycja Y w pikselach
     * @param upKey     Klawisz ruchu w górę
     * @param downKey   Klawisz ruchu w dół
     * @param leftKey   Klawisz ruchu w lewo
     * @param rightKey  Klawisz ruchu w prawo
     * @param attackKey Klawisz strzelania strzałą
     */
    Archer(float startX, float startY,
           sf::Keyboard::Key upKey,
           sf::Keyboard::Key downKey,
           sf::Keyboard::Key leftKey,
           sf::Keyboard::Key rightKey,
           sf::Keyboard::Key attackKey);

    /**
     * @brief Aktualizuje logikę łucznika
     *
     * Nadpisuje metodę bazową żeby zaimplementować strzelanie strzałami.
     * Obsługuje: ruch, cooldown strzelania, tworzenie strzał (isArrow=true),
     * aktualizację i usuwanie nieaktywnych strzał, pasek HP.
     * @param deltaTime  Czas od ostatniej klatki w sekundach
     * @param windowSize Rozmiar okna do ograniczenia ruchu
     */
    void update(float deltaTime, const sf::Vector2u& windowSize) override;

    /**
     * @brief Rysuje łucznika i aktywne strzały
     *
     * Wywołuje drawBase() a następnie rysuje wszystkie aktywne strzały.
     * @param window Referencja do okna gry
     */
    void draw(sf::RenderWindow& window) override;

private:
    static const float ARROW_SPEED; ///< Prędkość strzały w px/s (280)
};