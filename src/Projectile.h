/**
 * @file Projectile.h
 * @brief Deklaracja klasy Projectile reprezentującej pocisk lub strzałę
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Direction.h"

class Wall;

/**
 * @brief Reprezentuje lecący pocisk lub strzałę
 *
 * Klasa obsługuje dwa typy pocisków:
 * - Pocisk maga: fioletowe kółko, prędkość 400px/s
 * - Strzała łucznika: zielona kreska, prędkość 280px/s
 *
 * Pocisk porusza się w jednym kierunku aż trafi w gracza,
 * ścianę lub wyleci poza granice ekranu.
 */
class Projectile
{
public:
    /**
     * @brief Konstruktor pocisku
     * @param startX    Pozycja startowa X w pikselach
     * @param startY    Pozycja startowa Y w pikselach
     * @param direction Kierunek lotu pocisku
     * @param damage    Obrażenia zadawane przy trafieniu
     * @param speed     Prędkość pocisku w pikselach na sekundę (domyślnie 400)
     * @param isArrow   Czy to strzała łucznika (true) czy pocisk maga (false)
     */
    Projectile(float startX, float startY,
               Direction direction,
               int damage,
               float speed = 400.f,
               bool isArrow = false);

    /**
     * @brief Aktualizuje pozycję pocisku
     * @param deltaTime Czas od ostatniej klatki w sekundach
     */
    void update(float deltaTime);

    /**
     * @brief Rysuje pocisk na ekranie
     * @param window Referencja do okna gry
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Sprawdza czy pocisk jest aktywny
     * @return true jeśli pocisk nie trafił i nie wyleciał poza ekran
     */
    bool isActive() const;

    /**
     * @brief Zwraca prostokąt kolizji pocisku
     * @return Prostokąt do detekcji kolizji z graczem lub ścianą
     */
    sf::FloatRect getBounds() const;

    /**
     * @brief Zwraca obrażenia zadawane przez pocisk
     * @return Wartość obrażeń
     */
    int getDamage() const;

    /**
     * @brief Dezaktywuje pocisk po trafieniu w cel
     */
    void deactivate();

    /**
     * @brief Sprawdza kolizję pocisku z wektorem ścian
     *
     * Jeśli pocisk dotyka ściany, zostaje zdezaktywowany.
     * @param walls Wektor ścian na planszy
     */
    void checkWallCollisions(const std::vector<std::unique_ptr<Wall>>& walls);

private:
    sf::CircleShape    m_shape;      ///< Kształt pocisku maga (kółko)
    sf::RectangleShape m_arrowShape; ///< Kształt strzały łucznika (kreska)
    sf::Vector2f       m_velocity;   ///< Wektor prędkości (px/s w osiach X i Y)
    bool               m_active;     ///< Czy pocisk jest aktywny
    int                m_damage;     ///< Obrażenia przy trafieniu
    float              m_speed;      ///< Prędkość pocisku w px/s
    bool               m_isArrow;    ///< Czy to strzała (true) czy pocisk maga (false)
};