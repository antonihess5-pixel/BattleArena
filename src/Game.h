/**
 * @file Game.h
 * @brief Deklaracja klasy Game - głównego kontrolera gry
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include "PlayerBase.h"
#include "GameState.h"
#include "Wall.h"
#include "WallPlacer.h"
#include "HealthPack.h"

/**
 * @brief Główna klasa gry zarządzająca całą logiką i renderowaniem
 *
 * Klasa Game implementuje wzorzec "Game Loop" i zarządza:
 * - Oknem aplikacji SFML
 * - Przejściami między stanami gry (GameState)
 * - Wektorem graczy (polimorfizm przez PlayerBase*)
 * - Wektorem ścian na planszy
 * - Fazą układania ścian (WallPlacer)
 * - Wykrywaniem kolizji między graczami (checkCombat)
 * - Renderowaniem wszystkich ekranów gry
 *
 * Cykl życia stanu:
 * Menu → CharacterSelect → WallPlacement → Playing → GameOver → CharacterSelect
 */
class Game
{
public:
    /**
     * @brief Konstruktor gry
     *
     * Inicjalizuje okno SFML w trybie pełnoekranowym,
     * ładuje czcionkę systemową i tworzy domyślnych graczy.
     */
    Game();

    /**
     * @brief Uruchamia główną pętlę gry
     *
     * Pętla działa dopóki okno jest otwarte.
     * W każdej iteracji: mierzy deltaTime, przetwarza zdarzenia,
     * aktualizuje logikę i renderuje klatkę.
     */
    void run();

private:
    /**
     * @brief Przetwarza zdarzenia systemu (klawiatura, zamknięcie okna)
     *
     * Obsługuje różne zdarzenia zależnie od aktualnego stanu gry.
     * Przekazuje wciśnięcia klawiszy ataku do graczy przez onAttackKeyPressed().
     */
    void processEvents();

    /**
     * @brief Aktualizuje logikę gry
     *
     * Wywoływana tylko w stanie Playing. Aktualizuje graczy,
     * sprawdza kolizje ze ścianami i wykrywa koniec gry.
     * @param deltaTime Czas od ostatniej klatki w sekundach
     */
    void update(float deltaTime);

    /**
     * @brief Renderuje aktualny stan gry
     *
     * Wybiera odpowiednią metodę renderowania na podstawie m_state.
     */
    void render();

    /**
     * @brief Sprawdza kolizje bojowe między graczami
     *
     * Dla każdej pary graczy sprawdza:
     * - Kolizję hitboxa ataku wręcz (Warrior)
     * - Kolizje aktywnych pocisków z przeciwnikiem (Mage, Archer)
     * Używa wektora m_players co umożliwia obsługę dowolnej liczby graczy.
     */
    void checkCombat();

    /**
     * @brief Renderuje ekran głównego menu
     */
    void renderMenu();

    /**
     * @brief Renderuje ekran wyboru postaci
     *
     * Wyświetla karty postaci dla obu graczy z podglądem
     * kolorów i statystyk. Podświetla aktualnie wybraną klasę.
     */
    void renderCharacterSelect();

    /**
     * @brief Renderuje ekran końca gry
     *
     * Wyświetla półprzezroczyste tło i informację o zwycięzcy.
     * Sprawdza stan HP obu graczy żeby ustalić wynik.
     */
    void renderGameOver();

    /**
     * @brief Resetuje grę do stanu wyboru postaci
     *
     * Czyści wybory postaci, ściany i flagi gotowości graczy.
     * Przechodzi do stanu CharacterSelect.
     */
    void resetGame();

    /**
     * @brief Tworzy ściany dookoła areny
     *
     * Ściany brzegowe ograniczają pole walki i są tworzone
     * przed fazą układania ścian przez graczy.
     * Parametry areny muszą być identyczne jak w WallPlacer.
     */
    void createBorderWalls();

    /**
     * @brief Fabryka graczy - tworzy obiekt odpowiedniej klasy postaci
     *
     * Używa polimorfizmu - zwraca unique_ptr<PlayerBase> który
     * wskazuje na Warrior, Mage lub Archer zależnie od wyboru.
     * @param characterIndex Indeks wybranej klasy (0=Warrior, 1=Mage, 2=Archer)
     * @param startX         Startowa pozycja X
     * @param startY         Startowa pozycja Y
     * @param upKey          Klawisz ruchu w górę
     * @param downKey        Klawisz ruchu w dół
     * @param leftKey        Klawisz ruchu w lewo
     * @param rightKey       Klawisz ruchu w prawo
     * @param attackKey      Klawisz ataku
     * @return Wskaźnik do nowego obiektu gracza
     */
    std::unique_ptr<PlayerBase> createPlayer(
        int characterIndex,
        float startX, float startY,
        sf::Keyboard::Key upKey,
        sf::Keyboard::Key downKey,
        sf::Keyboard::Key leftKey,
        sf::Keyboard::Key rightKey,
        sf::Keyboard::Key attackKey);

    sf::RenderWindow m_window; ///< Okno gry SFML (tryb pełnoekranowy)
    sf::Clock        m_clock;  ///< Zegar do mierzenia deltaTime między klatkami

    /// Wektor graczy - polimorfizm pozwala trzymać różne klasy postaci
    std::vector<std::unique_ptr<PlayerBase>> m_players;

    /// Wektor wszystkich ścian na planszy (brzegowe + postawione przez graczy)
    std::vector<std::unique_ptr<Wall>> m_walls;

    /// Obiekt zarządzający fazą układania ścian (nullptr poza tą fazą)
    std::unique_ptr<WallPlacer> m_wallPlacer;

    GameState m_state; ///< Aktualny stan gry
    sf::Font  m_font;  ///< Czcionka systemowa Arial do wyświetlania tekstu

    std::vector<int>  m_playerChoices; ///< Wybrane klasy postaci (0=Warrior,1=Mage,2=Archer)
    std::vector<bool> m_playerReady;   ///< Flagi gotowości graczy na ekranie wyboru
    /**
     * @brief Aktualizuje apteczki - sprawdza kolizje z graczami
     */
    void updateHealthPacks();

    /**
     * @brief Próbuje spawnnować nową apteczkę na losowej pozycji
     */
    void spawnHealthPack();

    std::vector<HealthPack> m_healthPacks;  ///< Aktywne apteczki na planszy
    float m_healthPackTimer;                ///< Licznik czasu do spawnu apteczki
    float m_healthPackInterval;            ///< Co ile sekund pojawia się apteczka
};