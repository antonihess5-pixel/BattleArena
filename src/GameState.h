/**
 * @file GameState.h
 * @brief Definicja wyliczenia stanów gry
 */
 
#pragma once
 
/**
 * @brief Możliwe stany gry
 *
 * Gra przechodzi przez kolejne stany w następującej kolejności:
 * Menu → CharacterSelect → WallPlacement → Playing → GameOver
 * Z GameOver można wrócić do CharacterSelect przez restart.
 */
enum class GameState
{
    Menu,            ///< Ekran głównego menu
    CharacterSelect, ///< Ekran wyboru postaci dla obu graczy
    WallPlacement,   ///< Faza układania ścian na planszy przed walką
    Playing,         ///< Właściwa rozgrywka - walka między graczami
    GameOver         ///< Ekran końca gry z informacją o zwycięzcy
};
 