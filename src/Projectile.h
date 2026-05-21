#pragma once

#include <SFML/Graphics.hpp>
#include "Direction.h"

/**
 * @brief Pocisk maga - porusza się w jednym kierunku aż trafi lub wyleci poza ekran
 */
class Projectile
{
public:
    /**
     * @brief Konstruktor pocisku
     * @param startX    - pozycja startowa X
     * @param startY    - pozycja startowa Y
     * @param direction - kierunek lotu
     * @param damage    - obrażenia przy trafieniu
     */
    Projectile(float startX, float startY,
               Direction direction,
               int damage);

    // Aktualizuje pozycję pocisku
    void update(float deltaTime);

    // Rysuje pocisk
    void draw(sf::RenderWindow& window);

    // Czy pocisk jest aktywny (nie trafił i nie wyleciał)?
    bool isActive() const;

    // Prostokąt do detekcji kolizji
    sf::FloatRect getBounds() const;

    // Ile obrażeń zadaje
    int getDamage() const;

    // Dezaktywuj pocisk (gdy trafił w gracza)
    void deactivate();

private:
    sf::CircleShape m_shape;   // pocisk rysujemy jako kółko
    sf::Vector2f m_velocity;   // wektor prędkości (x, y)
    bool m_active;             // czy pocisk żyje
    int m_damage;              // obrażenia
    float m_speed;             // prędkość pocisku
};