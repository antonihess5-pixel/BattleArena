#include "Projectile.h"

Projectile::Projectile(float startX, float startY,
                       Direction direction,
                       int damage)
    : m_active(true)
    , m_damage(damage)
    , m_speed(400.f)  // pocisk leci szybko
{
    // Wygląd pocisku - małe fioletowe kółko
    m_shape.setRadius(8.f);
    m_shape.setFillColor(sf::Color(220, 100, 255));
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color::White);
    m_shape.setPosition(startX, startY);

    // Ustawiamy wektor prędkości zależnie od kierunku
    // Wektor prędkości to para (vx, vy) - ile pikseli na sekundę
    // w każdej osi. Np. lecąc w prawo: vx = 400, vy = 0
    switch (direction)
    {
        case Direction::Up:
            m_velocity = { 0.f, -m_speed };
            break;
        case Direction::Down:
            m_velocity = { 0.f, m_speed };
            break;
        case Direction::Left:
            m_velocity = { -m_speed, 0.f };
            break;
        case Direction::Right:
            m_velocity = { m_speed, 0.f };
            break;
    }
}

void Projectile::update(float deltaTime)
{
    if (!m_active)
        return;

    // Przesuwamy pocisk zgodnie z wektorem prędkości
    m_shape.move(m_velocity * deltaTime);

    // Dezaktywuj jeśli wyleciał poza ekran
    sf::Vector2f pos = m_shape.getPosition();
    if (pos.x < -50.f || pos.x > 900.f ||
        pos.y < -50.f || pos.y > 700.f)
    {
        m_active = false;
    }
}

void Projectile::draw(sf::RenderWindow& window)
{
    if (m_active)
        window.draw(m_shape);
}

bool Projectile::isActive() const
{
    return m_active;
}

sf::FloatRect Projectile::getBounds() const
{
    return m_shape.getGlobalBounds();
}

int Projectile::getDamage() const
{
    return m_damage;
}

void Projectile::deactivate()
{
    m_active = false;
}