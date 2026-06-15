/**
 * @file WallPlacer.h
 * @brief Deklaracja klasy WallPlacer zarządzającej fazą układania ścian
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Wall.h"

/**
 * @brief Zarządza fazą układania ścian przed walką
 *
 * Klasa wyświetla siatkę 16x16 na ekranie i pozwala graczom
 * na zmianę stawiać ściany na krawędziach kratek.
 *
 * Sterowanie:
 * - WSAD: ruch kursora po węzłach siatki
 * - Strzałki: postawienie ściany w danym kierunku od kursora
 *
 * Po postawieniu ściany tura przechodzi automatycznie do drugiego gracza.
 * Faza kończy się gdy obaj gracze postawią wszystkie dostępne ściany.
 */
class WallPlacer
{
public:
    /**
     * @brief Konstruktor WallPlacer
     * @param wallsPerPlayer Liczba ścian do postawienia przez każdego gracza
     * @param windowWidth    Szerokość okna gry w pikselach
     * @param windowHeight   Wysokość okna gry w pikselach
     */
    WallPlacer(int wallsPerPlayer, float windowWidth, float windowHeight);

    /**
     * @brief Obsługuje wciśnięcie klawisza przez gracza
     *
     * Przetwarza klawisze ruchu kursora (WSAD) oraz stawiania ścian (strzałki).
     * @param key Kod wciśniętego klawisza SFML
     * @return true jeśli faza układania ścian dobiegła końca
     */
    bool handleInput(sf::Keyboard::Key key);

    /**
     * @brief Rysuje siatkę, ściany, kursor i informacje UI
     * @param window Referencja do okna gry
     * @param font   Referencja do czcionki używanej do tekstu UI
     */
    void draw(sf::RenderWindow& window, sf::Font& font) const;

    /**
     * @brief Zwraca wektor postawionych ścian
     *
     * Wywoływane przez Game po zakończeniu fazy żeby przenieść
     * ściany do właściwej rozgrywki.
     * @return Stała referencja do wektora unique_ptr ścian
     */
    const std::vector<std::unique_ptr<Wall>>& getWalls() const;

    /**
     * @brief Zwraca offset siatki w osi X
     * @return Przesunięcie siatki od lewej krawędzi ekranu w pikselach
     */
    float getOffsetX() const { return m_offsetX; }

    /**
     * @brief Zwraca offset siatki w osi Y
     * @return Przesunięcie siatki od górnej krawędzi ekranu w pikselach
     */
    float getOffsetY() const { return m_offsetY; }

    /**
     * @brief Zwraca rozmiar jednej kratki siatki
     * @return Rozmiar kratki w pikselach
     */
    float getCellSize() const { return m_cellSize; }

private:
    /**
     * @brief Próbuje postawić ścianę między kursorem a sąsiednim węzłem
     * @param dx Przesunięcie w osi X (-1, 0 lub 1)
     * @param dy Przesunięcie w osi Y (-1, 0 lub 1)
     */
    void placeWall(int dx, int dy);

    /**
     * @brief Sprawdza czy węzeł siatki mieści się w granicach planszy
     * @param x Kolumna węzła
     * @param y Wiersz węzła
     * @return true jeśli węzeł jest poprawny (0..GRID_SIZE)
     */
    bool isValidNode(int x, int y) const;

    static const int GRID_SIZE = 16; ///< Liczba kratek siatki w każdym wymiarze

    float m_cellSize; ///< Rozmiar jednej kratki w pikselach
    float m_offsetX;  ///< Przesunięcie siatki od lewej krawędzi ekranu
    float m_offsetY;  ///< Przesunięcie siatki od górnej krawędzi ekranu

    int m_cursorX;    ///< Kolumna węzła na którym stoi kursor (0..GRID_SIZE)
    int m_cursorY;    ///< Wiersz węzła na którym stoi kursor (0..GRID_SIZE)

    std::vector<std::unique_ptr<Wall>> m_walls; ///< Postawione ściany

    int m_wallsPerPlayer;   ///< Ile ścian może postawić każdy gracz
    int m_wallsPlaced;      ///< Łączna liczba postawionych ścian
    int m_currentPlayer;    ///< Indeks gracza którego tura (0 lub 1)
};