#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Klasa reprezentująca gracza w grze
 * 
 * Każdy gracz ma swoją pozycję, kolor i sterowanie
 * przypisane do innych klawiszy
 */
class Player
{
public:
    /**
     * @brief Konstruktor gracza
     * @param startX    - startowa pozycja X
     * @param startY    - startowa pozycja Y
     * @param color     - kolor prostokąta gracza
     * @param upKey     - klawisz ruchu w górę
     * @param downKey   - klawisz ruchu w dół
     * @param leftKey   - klawisz ruchu w lewo
     * @param rightKey  - klawisz ruchu w prawo
     */
    Player(float startX, float startY,
           sf::Color color,
           sf::Keyboard::Key upKey,
           sf::Keyboard::Key downKey,
           sf::Keyboard::Key leftKey,
           sf::Keyboard::Key rightKey);

    // Aktualizuje logikę gracza (ruch)
    void update(float deltaTime);

    // Rysuje gracza na ekranie
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape m_shape;   // prostokąt gracza

    // Klawisze sterowania - każdy gracz ma inne
    sf::Keyboard::Key m_upKey;
    sf::Keyboard::Key m_downKey;
    sf::Keyboard::Key m_leftKey;
    sf::Keyboard::Key m_rightKey;

    float m_speed;  // prędkość gracza w pikselach na sekundę
};