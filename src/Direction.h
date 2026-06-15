/**
 * @file Direction.h
 * @brief Definicja wyliczenia kierunków ruchu gracza
 */

#pragma once

/**
 * @brief Kierunek w którym patrzy gracz
 *
 * Używane do określenia kierunku ruchu oraz kierunku ataku.
 * Wartość jest aktualizowana przy każdym wciśnięciu klawisza ruchu.
 */
enum class Direction
{
    Up,    ///< Kierunek: góra
    Down,  ///< Kierunek: dół
    Left,  ///< Kierunek: lewo
    Right  ///< Kierunek: prawo
};