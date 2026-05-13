#pragma once

#include <SFML/Graphics.hpp>
#include <memory>        // dla std::unique_ptr
#include "PlayerBase.h"
#include "GameState.h"

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
    void renderCharacterSelect();   // rysuje ekran wyboru postaci
    void renderGameOver();          // rysuje ekran końca gry
    void resetGame();               // resetuje grę do stanu początkowego

    // Tworzy gracza wybranego typu
    // unique_ptr to "inteligentny wskaźnik" - sam zwalnia pamięć
    std::unique_ptr<PlayerBase> createPlayer(
        int characterIndex,
        float startX, float startY,
        sf::Keyboard::Key upKey,
        sf::Keyboard::Key downKey,
        sf::Keyboard::Key leftKey,
        sf::Keyboard::Key rightKey,
        sf::Keyboard::Key attackKey
    );

    // Pola prywatne - dane klasy
    sf::RenderWindow m_window;  // okno gry (m_ = member, konwencja nazewnictwa)
    sf::Clock m_clock;          // zegar do mierzenia deltaTime

    // unique_ptr zamiast zwykłych obiektów - typ gracza zależy od wyboru
    std::unique_ptr<PlayerBase> m_player1;
    std::unique_ptr<PlayerBase> m_player2;

    GameState m_state;          // aktualny stan gry
    sf::Font m_font;            // czcionka do wyświetlania tekstu

    // Wybory postaci (0 = Warrior, 1 = Mage)
    int m_player1Choice;
    int m_player2Choice;

    // Czy dany gracz już wybrał postać
    bool m_player1Ready;
    bool m_player2Ready;
};