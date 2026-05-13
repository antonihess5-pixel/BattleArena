#pragma once

/**
 * @brief Możliwe stany gry
 * 
 * enum class to "wyliczenie" - lista nazwanych wartości.
 * Używamy go zamiast zwykłych liczb (0, 1, 2) bo kod
 * jest wtedy czytelniejszy i bezpieczniejszy.
 */
enum class GameState
{
    Menu,       // ekran startowy
    Playing,    // rozgrywka
    GameOver    // ekran końca gry
};