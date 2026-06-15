/**
 * @file Wall.h
 * @brief Deklaracja klasy Wall reprezentującej ścianę na planszy
 */

#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Reprezentuje pojedynczą ścianę na planszy
 *
 * Ściana leży na krawędzi między dwoma węzłami siatki i może być:
 * - Pozioma: węzły mają takie samo Y, różne X
 * - Pionowa: węzły mają takie samo X, różne Y
 *
 * Ściany blokują ruch graczy i niszczą pociski przy kontakcie.
 * Pozycja ściany w pikselach jest obliczana na podstawie
 * współrzędnych węzłów, rozmiaru kratki i offsetu siatki.
 */
class Wall
{
public:
    /**
     * @brief Konstruktor ściany
     * @param x1       Kolumna pierwszego węzła siatki (0..16)
     * @param y1       Wiersz pierwszego węzła siatki (0..16)
     * @param x2       Kolumna drugiego węzła siatki (0..16)
     * @param y2       Wiersz drugiego węzła siatki (0..16)
     * @param cellSize Rozmiar jednej kratki siatki w pikselach
     * @param offsetX  Przesunięcie siatki od lewej krawędzi ekranu (domyślnie 0)
     * @param offsetY  Przesunięcie siatki od górnej krawędzi ekranu (domyślnie 0)
     */
    Wall(int x1, int y1, int x2, int y2,
         float cellSize,
         float offsetX = 0.f,
         float offsetY = 0.f);

    /**
     * @brief Konstruktor kopiujący
     *
     * Wymagany do przenoszenia ścian z WallPlacer do Game po zakończeniu
     * fazy układania. Kopiuje wszystkie pola włącznie z kształtem SFML.
     */
    Wall(const Wall& other) = default;

    /**
     * @brief Rysuje ścianę na ekranie
     * @param window Referencja do okna gry
     */
    void draw(sf::RenderWindow& window) const;

    /**
     * @brief Zwraca prostokąt kolizji ściany
     * @return Prostokąt używany do detekcji kolizji z graczem i pociskami
     */
    sf::FloatRect getBounds() const;

    /**
     * @brief Sprawdza czy ściana zajmuje podaną pozycję w siatce
     *
     * Sprawdzanie jest symetryczne: ściana A→B jest taka sama jak B→A.
     * Używane aby zapobiec postawieniu dwóch ścian w tym samym miejscu.
     * @param x1 Kolumna pierwszego węzła
     * @param y1 Wiersz pierwszego węzła
     * @param x2 Kolumna drugiego węzła
     * @param y2 Wiersz drugiego węzła
     * @return true jeśli ściana leży między tymi węzłami
     */
    bool isSamePosition(int x1, int y1, int x2, int y2) const;

private:
    sf::RectangleShape m_shape; ///< Prostokąt reprezentujący ścianę
    int m_x1;                  ///< Kolumna pierwszego węzła siatki
    int m_y1;                  ///< Wiersz pierwszego węzła siatki
    int m_x2;                  ///< Kolumna drugiego węzła siatki
    int m_y2;                  ///< Wiersz drugiego węzła siatki
};