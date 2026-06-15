/**
 * @file Archer.cpp
 * @brief Implementacja klasy Archer
 */

#include "Archer.h"

// Strzała łucznika leci wolniej niż pocisk maga (400px/s)
// ale zadaje więcej obrażeń i jest większa (łatwiej trafić)
const float Archer::ARROW_SPEED = 280.f;

Archer::Archer(float startX, float startY,
               sf::Keyboard::Key upKey,
               sf::Keyboard::Key downKey,
               sf::Keyboard::Key leftKey,
               sf::Keyboard::Key rightKey,
               sf::Keyboard::Key attackKey)
    : PlayerBase(startX, startY,
                 sf::Color(50, 200, 80), // zielony
                 upKey, downKey, leftKey, rightKey, attackKey,
                 110,   // HP - średnie
                 200.f, // prędkość - średnia
                 20,    // obrażenia - średnie
                 0.f)   // zasięg wręcz nieużywany - łucznik strzela
{
    // Łucznik jest smuklejszy od wojownika
    m_shape.setSize(sf::Vector2f(35.f, 40.f));
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color(20, 120, 40));

    // Schowaj hitbox ataku wręcz - łucznik go nie używa
    m_attackShape.setPosition(-1000.f, -1000.f);

    // Łucznik strzela wolniej niż mag ze względu na balans
    m_attackCooldown = 0.6f;
}

void Archer::update(float deltaTime, const sf::Vector2u& windowSize)
{
    // --- RUCH (taki sam jak w PlayerBase) ---
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(m_upKey))
    {
        movement.y -= m_speed * deltaTime;
        m_direction = Direction::Up;
    }
    if (sf::Keyboard::isKeyPressed(m_downKey))
    {
        movement.y += m_speed * deltaTime;
        m_direction = Direction::Down;
    }
    if (sf::Keyboard::isKeyPressed(m_leftKey))
    {
        movement.x -= m_speed * deltaTime;
        m_direction = Direction::Left;
    }
    if (sf::Keyboard::isKeyPressed(m_rightKey))
    {
        movement.x += m_speed * deltaTime;
        m_direction = Direction::Right;
    }

    m_shape.move(movement);
    clampToWindow(windowSize);

    // --- SYSTEM STRZAŁ ---
    if (m_attackCooldownTimer > 0.f)
        m_attackCooldownTimer -= deltaTime;

    // Strzelanie event-based przez flagę m_attackKeyPressed
    if (m_attackKeyPressed && m_attackCooldownTimer <= 0.f)
    {
        // Strzała startuje ze środka postaci łucznika
        sf::Vector2f pos = m_shape.getPosition();
        float startX = pos.x + m_shape.getSize().x / 2.f;
        float startY = pos.y + m_shape.getSize().y / 2.f;

        // isArrow=true = zielona kreska zamiast fioletowego kółka
        m_projectiles.push_back(
            std::make_unique<Projectile>(
                startX, startY,
                m_direction,
                m_damage,
                ARROW_SPEED,
                true) // true = strzała łucznika
        );

        m_attackCooldownTimer = m_attackCooldown;
        m_attacking = true;
        m_hitDealt  = false;
    }
    else
    {
        m_attacking = false;
    }

    // Aktualizuj wszystkie aktywne strzały
    for (auto& p : m_projectiles)
        p->update(deltaTime);

    // Usuń nieaktywne strzały
    m_projectiles.erase(
        std::remove_if(m_projectiles.begin(), m_projectiles.end(),
            [](const std::unique_ptr<Projectile>& p)
            { return !p->isActive(); }),
        m_projectiles.end()
    );

    // --- AKTUALIZACJA PASKA HP ---
    sf::Vector2f pos = m_shape.getPosition();
    m_hpBar.setPosition(pos.x, pos.y - 10.f);
    m_hpBarFill.setPosition(pos.x, pos.y - 10.f);

    float hpPercent = static_cast<float>(m_hp) / m_maxHP;
    m_hpBarFill.setSize(sf::Vector2f(40.f * hpPercent, 6.f));

    if (hpPercent > 0.5f)
        m_hpBarFill.setFillColor(sf::Color::Green);
    else if (hpPercent > 0.25f)
        m_hpBarFill.setFillColor(sf::Color::Yellow);
    else
        m_hpBarFill.setFillColor(sf::Color::Red);

    // Reset flagi - musi być na końcu klatki
    m_attackKeyPressed = false;
}

void Archer::draw(sf::RenderWindow& window)
{
    // Rysuj wspólne elementy (kształt, HP bar)
    drawBase(window);

    // Rysuj wszystkie aktywne strzały
    for (auto& p : m_projectiles)
        p->draw(window);
}