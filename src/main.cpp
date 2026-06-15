/**
 * @file main.cpp
 * @brief Punkt wejścia programu BattleArena
 *
 * Tworzy obiekt głównej klasy Game i uruchamia pętlę gry.
 * Cała logika gry znajduje się w klasie Game i klasach postaci.
 */

#include "Game.h"

/**
 * @brief Główna funkcja programu
 *
 * Punkt wejścia aplikacji. Tworzy obiekt Game który
 * inicjalizuje okno SFML i uruchamia główną pętlę gry.
 * Program kończy się gdy użytkownik zamknie okno lub naciśnie Escape.
 * @return 0 przy poprawnym zakończeniu programu
 */
int main()
{
    Game game;
    game.run();
    return 0;
}