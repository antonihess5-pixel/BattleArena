#pragma once

/**
 * @brief Kierunek w którym patrzy gracz
 * 
 * Używamy enum class zamiast liczb dla czytelności kodu.
 * Kierunek zmienia się gdy gracz wciska klawisz ruchu.
 */
enum class Direction
{
    Up,
    Down,
    Left,
    Right
};