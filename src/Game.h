#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "GameState.h"  // nowy plik ze stanami

/**
 * @brief Główna klasa gry - zarządza oknem, pętlą gry i stanami
 */
class Game
{
public:
    // Konstruktor - inicjalizuje grę
    Game();

    // Główna metoda - uruchamia pętlę gry
    void run();

private:
    // Metody prywatne - tylko Game może je wywołać
    void processEvents();           // obsługa klawiatury/myszy
    void update(float deltaTime);   // logika gry
    void render();                  // rysowanie
    void checkCombat();             // sprawdza czy ktoś kogoś trafił

    // Metody dla poszczególnych stanów
    void renderMenu();              // rysuje ekran startowy
    void renderGameOver();          // rysuje ekran końca gry
    void resetGame();               // resetuje grę do stanu początkowego

    // Pola prywatne - dane klasy
    sf::RenderWindow m_window;  // okno gry (m_ = member, konwencja nazewnictwa)
    sf::Clock m_clock;          // zegar do mierzenia deltaTime

    Player m_player1;           // gracz 1
    Player m_player2;           // gracz 2

    GameState m_state;          // aktualny stan gry
    sf::Font m_font;            // czcionka do wyświetlania tekstu
};