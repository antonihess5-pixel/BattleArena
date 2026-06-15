/**
 * @file Mage.h
 * @brief Deklaracja klasy Mage - mag strzelający pociskami magicznymi
 */

#pragma once

#include "PlayerBase.h"

/**
 * @brief Mag - klasa postaci walczącej na dystans pociskami magicznymi
 *
 * Dziedziczy po PlayerBase i nadpisuje metody update() oraz draw()
 * aby zaimplementować własny system strzelania pociskami.
 *
 * Mag nie używa ataku wręcz - zamiast tego strzela fioletowymi
 * kółkami z prędkością 400px/s. Posiada najniższe HP ale
 * najwyższą prędkość ruchu.
 *
 * Statystyki:
 * - HP: 80 (najniższe)
 * - Prędkość: 250 px/s (najwyższa)
 * - Obrażenia: 15 (najniższe)
 * - Zasięg: daleki (pociski 400px/s, cooldown 0.3s)
 */
class Mage : public PlayerBase
{
public:
    /**
     * @brief Konstruktor maga
     *
     * Statystyki są zdefiniowane wewnątrz klasy i nie mogą być zmienione.
     * @param startX    Startowa pozycja X w pikselach
     * @param startY    Startowa pozycja Y w pikselach
     * @param upKey     Klawisz ruchu w górę
     * @param downKey   Klawisz ruchu w dół
     * @param leftKey   Klawisz ruchu w lewo
     * @param rightKey  Klawisz ruchu w prawo
     * @param attackKey Klawisz strzelania pociskiem
     */
    Mage(float startX, float startY,
         sf::Keyboard::Key upKey,
         sf::Keyboard::Key downKey,
         sf::Keyboard::Key leftKey,
         sf::Keyboard::Key rightKey,
         sf::Keyboard::Key attackKey);

    /**
     * @brief Aktualizuje logikę maga
     *
     * Nadpisuje metodę bazową żeby zaimplementować własny system
     * strzelania pociskami zamiast ataku wręcz.
     * Obsługuje: ruch, cooldown strzelania, tworzenie pocisków,
     * aktualizację i usuwanie nieaktywnych pocisków, pasek HP.
     * @param deltaTime  Czas od ostatniej klatki w sekundach
     * @param windowSize Rozmiar okna do ograniczenia ruchu
     */
    void update(float deltaTime, const sf::Vector2u& windowSize) override;

    /**
     * @brief Rysuje maga, efekty aury i aktywne pociski
     *
     * Wywołuje drawBase() a następnie rysuje fioletową aurę
     * podczas ataku i wszystkie aktywne pociski.
     * @param window Referencja do okna gry
     */
    void draw(sf::RenderWindow& window) override;
};