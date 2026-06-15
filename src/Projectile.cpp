/**
 * @file Projectile.cpp
 * @brief Implementacja klasy Projectile
 */

#include "Projectile.h"
#include "Wall.h"

Projectile::Projectile(float startX, float startY,
                       Direction direction,
                       int damage,
                       float speed,
                       bool isArrow)
    : m_active(true)
    , m_damage(damage)
    , m_speed(speed)
    , m_isArrow(isArrow)
{
    if (isArrow)
    {
        // Strzała łucznika - zielona kreska w kierunku lotu
        if (direction == Direction::Left || direction == Direction::Right)
            m_arrowShape.setSize(sf::Vector2f(20.f, 5.f));  // pozioma
        else
            m_arrowShape.setSize(sf::Vector2f(5.f, 20.f));  // pionowa

        m_arrowShape.setFillColor(sf::Color(50, 220, 80));
        m_arrowShape.setOutlineThickness(1.f);
        m_arrowShape.setOutlineColor(sf::Color(20, 150, 40));
        m_arrowShape.setPosition(startX, startY);
    }
    else
    {
        // Pocisk maga - fioletowe kółko
        m_shape.setRadius(8.f);
        m_shape.setFillColor(sf::Color(220, 100, 255));
        m_shape.setOutlineThickness(2.f);
        m_shape.setOutlineColor(sf::Color::White);
        m_shape.setPosition(startX, startY);
    }

    // Wektor prędkości zależnie od kierunku
    switch (direction)
    {
        case Direction::Up:    m_velocity = { 0.f,    -m_speed }; break;
        case Direction::Down:  m_velocity = { 0.f,     m_speed }; break;
        case Direction::Left:  m_velocity = {-m_speed, 0.f     }; break;
        case Direction::Right: m_velocity = { m_speed, 0.f     }; break;
    }
}

void Projectile::update(float deltaTime)
{
    if (!m_active) return;

    if (m_isArrow)
    {
        m_arrowShape.move(m_velocity * deltaTime);
        sf::Vector2f pos = m_arrowShape.getPosition();
        if (pos.x < -50.f || pos.x > 2000.f ||
            pos.y < -50.f || pos.y > 1500.f)
            m_active = false;
    }
    else
    {
        m_shape.move(m_velocity * deltaTime);
        sf::Vector2f pos = m_shape.getPosition();
        if (pos.x < -50.f || pos.x > 2000.f ||
            pos.y < -50.f || pos.y > 1500.f)
            m_active = false;
    }
}

void Projectile::draw(sf::RenderWindow& window)
{
    if (!m_active) return;

    if (m_isArrow)
        window.draw(m_arrowShape);
    else
        window.draw(m_shape);
}

bool Projectile::isActive() const { return m_active; }

sf::FloatRect Projectile::getBounds() const
{
    if (m_isArrow)
        return m_arrowShape.getGlobalBounds();
    return m_shape.getGlobalBounds();
}

int Projectile::getDamage() const { return m_damage; }

void Projectile::deactivate() { m_active = false; }

void Projectile::checkWallCollisions(
    const std::vector<std::unique_ptr<Wall>>& walls)
{
    if (!m_active) return;

    for (auto& wall : walls)
    {
        if (getBounds().intersects(wall->getBounds()))
        {
            m_active = false;
            return;
        }
    }
}