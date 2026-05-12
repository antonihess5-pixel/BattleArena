#include "Player.h"

// Konstruktor - inicjalizujemy wszystkie pola
Player::Player(float startX, float startY,
               sf::Color color,
               sf::Keyboard::Key upKey,
               sf::Keyboard::Key downKey,
               sf::Keyboard::Key leftKey,
               sf::Keyboard::Key rightKey)
    : m_upKey(upKey)
    , m_downKey(downKey)
    , m_leftKey(leftKey)
    , m_rightKey(rightKey)
    , m_speed(200.f)  // 200 pikseli na sekundę
{
    // Ustawiamy wygląd prostokąta
    m_shape.setSize(sf::Vector2f(40.f, 40.f));  // 40x40 pikseli
    m_shape.setFillColor(color);
    m_shape.setPosition(startX, startY);
}

void Player::update(float deltaTime)
{
    // deltaTime to czas który minął od ostatniej klatki
    // Dzięki temu ruch jest płynny niezależnie od FPS
    sf::Vector2f movement(0.f, 0.f);

    // Sprawdzamy czy dany klawisz jest wciśnięty
    if (sf::Keyboard::isKeyPressed(m_upKey))
        movement.y -= m_speed * deltaTime;

    if (sf::Keyboard::isKeyPressed(m_downKey))
        movement.y += m_speed * deltaTime;

    if (sf::Keyboard::isKeyPressed(m_leftKey))
        movement.x -= m_speed * deltaTime;

    if (sf::Keyboard::isKeyPressed(m_rightKey))
        movement.x += m_speed * deltaTime;

    // Przesuwamy prostokąt o obliczony wektor ruchu
    m_shape.move(movement);
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}