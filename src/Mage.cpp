#include "Mage.h"

/**
 * Konstruktor maga - wywołuje konstruktor klasy bazowej
 * z ustalonymi statystykami maga.
 */
Mage::Mage(float startX, float startY,
           sf::Keyboard::Key upKey,
           sf::Keyboard::Key downKey,
           sf::Keyboard::Key leftKey,
           sf::Keyboard::Key rightKey,
           sf::Keyboard::Key attackKey)
    : PlayerBase(startX, startY,
                 sf::Color(180, 50, 255),  // fioletowy
                 upKey, downKey, leftKey, rightKey, attackKey,
                 80,     // HP - mało!
                 250.f,  // prędkość - szybki!
                 15,     // obrażenia
                 80.f)   // zasięg (nieużywany - mag strzela pociskami)
{
    // Mag jest mniejszy i smuklejszy
    m_shape.setSize(sf::Vector2f(30.f, 35.f));

    // Obramowanie w kolorze magii
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color(220, 150, 255));

    // Mag nie używa m_attackShape - schowaj go na stałe
    m_attackShape.setPosition(-1000.f, -1000.f);
}

void Mage::update(float deltaTime, const sf::Vector2u& windowSize)
{
    // Wywołujemy update klasy bazowej (ruch, granice, HP bar)
    // ale NIE updateAttack - mag ma własny system ataku
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

    // --- SYSTEM POCISKÓW ---
    // Odliczaj cooldown
    if (m_attackCooldownTimer > 0.f)
        m_attackCooldownTimer -= deltaTime;

    // Wciśnięto klawisz ataku i cooldown minął - stwórz pocisk!
    if (sf::Keyboard::isKeyPressed(m_attackKey) &&
        m_attackCooldownTimer <= 0.f)
    {
        // Pozycja startowa pocisku - środek maga
        sf::Vector2f pos = m_shape.getPosition();
        float startX = pos.x + m_shape.getSize().x / 2.f;
        float startY = pos.y + m_shape.getSize().y / 2.f;

        // Dodaj nowy pocisk do wektora
        // make_unique tworzy obiekt Projectile i zwraca unique_ptr
        m_projectiles.push_back(
            std::make_unique<Projectile>(startX, startY, m_direction, m_damage)
        );

        m_attackCooldownTimer = m_attackCooldown;
        m_attacking = true;
        m_hitDealt = false;
    }
    else
    {
        m_attacking = false;
    }

    // Aktualizuj wszystkie pociski
    for (auto& projectile : m_projectiles)
        projectile->update(deltaTime);

    // Usuń nieaktywne pociski z wektora
    // remove_if przesuwa nieaktywne na koniec, erase usuwa je
    m_projectiles.erase(
        std::remove_if(m_projectiles.begin(), m_projectiles.end(),
            [](const std::unique_ptr<Projectile>& p) { return !p->isActive(); }),
        m_projectiles.end()
    );

    // Aktualizuj pasek HP
    sf::Vector2f pos = m_shape.getPosition();
    m_hpBar.setPosition(pos.x, pos.y - 10.f);
    m_hpBarFill.setPosition(pos.x, pos.y - 10.f);

    float hpPercent = static_cast<float>(m_hp) / m_maxHP;
    m_hpBarFill.setSize(sf::Vector2f(40.f * hpPercent, 6.f));

    // Kolor paska zmienia się wraz z HP
    if (hpPercent > 0.5f)
        m_hpBarFill.setFillColor(sf::Color::Green);
    else if (hpPercent > 0.25f)
        m_hpBarFill.setFillColor(sf::Color::Yellow);
    else
        m_hpBarFill.setFillColor(sf::Color::Red);
}

void Mage::draw(sf::RenderWindow& window)
{
    // Rysujemy gracza i pasek HP
    window.draw(m_shape);
    window.draw(m_hpBar);
    window.draw(m_hpBarFill);

    // Mag ma dodatkowy efekt - małe kółko "aury" gdy atakuje
    if (m_attacking)
    {
        sf::CircleShape aura(25.f);
        aura.setFillColor(sf::Color(180, 50, 255, 40)); // bardzo przezroczysty
        aura.setPosition(
            m_shape.getPosition().x - 7.f,
            m_shape.getPosition().y - 7.f
        );
        window.draw(aura);
    }

    // Rysujemy wszystkie aktywne pociski
    for (auto& projectile : m_projectiles)
        projectile->draw(window);
}