/**
 * @file PlayerBase.cpp
 * @brief Implementacja klasy PlayerBase
 */

#include "PlayerBase.h"
#include "Wall.h"
#include <algorithm>

PlayerBase::PlayerBase(float startX, float startY,
               sf::Color color,
               sf::Keyboard::Key upKey,
               sf::Keyboard::Key downKey,
               sf::Keyboard::Key leftKey,
               sf::Keyboard::Key rightKey,
               sf::Keyboard::Key attackKey,
               int hp,
               float speed,
               int damage,
               float attackRange)
    : m_upKey(upKey)
    , m_downKey(downKey)
    , m_leftKey(leftKey)
    , m_rightKey(rightKey)
    , m_attackKey(attackKey)
    , m_speed(speed)
    , m_hp(hp)
    , m_maxHP(hp)
    , m_damage(damage)
    , m_attackRange(attackRange)
    , m_direction(Direction::Right)
    , m_attacking(false)
    , m_attackTimer(0.f)
    , m_attackDuration(0.2f)     // atak wręcz trwa 0.2 sekundy
    , m_attackCooldown(0.5f)     // cooldown między atakami
    , m_attackCooldownTimer(0.f)
    , m_hitDealt(false)
    , m_attackKeyPressed(false)
{
    m_shape.setSize(sf::Vector2f(40.f, 40.f));
    m_shape.setFillColor(color);
    m_shape.setPosition(startX, startY);

    // Hitbox ataku wręcz - rozmiar zależy od zasięgu klasy
    m_attackShape.setSize(sf::Vector2f(attackRange, attackRange * 0.75f));
    m_attackShape.setFillColor(sf::Color(255, 255, 0, 180));
    m_attackShape.setPosition(-1000.f, -1000.f); // schowany poza ekranem

    // Pasek HP - tło (ciemnoczerwone)
    m_hpBar.setSize(sf::Vector2f(40.f, 6.f));
    m_hpBar.setFillColor(sf::Color(150, 0, 0));

    // Pasek HP - wypełnienie (kolor zmienia się z HP)
    m_hpBarFill.setSize(sf::Vector2f(40.f, 6.f));
    m_hpBarFill.setFillColor(sf::Color::Green);
}

void PlayerBase::update(float deltaTime, const sf::Vector2u& windowSize)
{
    // --- RUCH ---
    // deltaTime zapewnia płynny ruch niezależnie od liczby klatek na sekundę
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
    updateAttack(deltaTime);

    // --- AKTUALIZACJA PASKA HP ---
    sf::Vector2f pos = m_shape.getPosition();
    m_hpBar.setPosition(pos.x, pos.y - 10.f);
    m_hpBarFill.setPosition(pos.x, pos.y - 10.f);

    float hpPercent = static_cast<float>(m_hp) / m_maxHP;
    m_hpBarFill.setSize(sf::Vector2f(40.f * hpPercent, 6.f));

    // Kolor paska zależy od pozostałego HP
    if (hpPercent > 0.5f)
        m_hpBarFill.setFillColor(sf::Color::Green);
    else if (hpPercent > 0.25f)
        m_hpBarFill.setFillColor(sf::Color::Yellow);
    else
        m_hpBarFill.setFillColor(sf::Color::Red);

    // Reset flagi ataku - musi być na końcu klatki
    m_attackKeyPressed = false;
}

void PlayerBase::updateAttack(float deltaTime)
{
    // Odliczanie cooldownu między atakami
    if (m_attackCooldownTimer > 0.f)
        m_attackCooldownTimer -= deltaTime;

    // Rozpoczęcie ataku gdy klawisz wciśnięty i cooldown minął
    if (m_attackKeyPressed && !m_attacking && m_attackCooldownTimer <= 0.f)
    {
        m_attacking = true;
        m_attackTimer = m_attackDuration;
        m_hitDealt = false;
    }

    if (m_attacking)
    {
        m_attackTimer -= deltaTime;

        // Pozycjonowanie hitboxa w kierunku patrzenia gracza
        sf::Vector2f pos = m_shape.getPosition();
        sf::Vector2f attackPos;
        float range = m_attackShape.getSize().x;

        switch (m_direction)
        {
            case Direction::Up:
                attackPos = { pos.x + 5.f, pos.y - range };
                break;
            case Direction::Down:
                attackPos = { pos.x + 5.f, pos.y + 45.f };
                break;
            case Direction::Left:
                attackPos = { pos.x - range, pos.y + 5.f };
                break;
            case Direction::Right:
                attackPos = { pos.x + 45.f, pos.y + 5.f };
                break;
        }

        m_attackShape.setPosition(attackPos);

        // Koniec animacji ataku
        if (m_attackTimer <= 0.f)
        {
            m_attacking = false;
            m_attackCooldownTimer = m_attackCooldown;
            m_attackShape.setPosition(-1000.f, -1000.f); // schowaj hitbox
        }
    }
}

void PlayerBase::clampToWindow(const sf::Vector2u& windowSize)
{
    sf::Vector2f pos  = m_shape.getPosition();
    sf::Vector2f size = m_shape.getSize();

    // Ograniczenie do granic okna w każdej osi
    if (pos.x < 0.f)                          pos.x = 0.f;
    if (pos.x + size.x > windowSize.x)        pos.x = windowSize.x - size.x;
    if (pos.y < 0.f)                           pos.y = 0.f;
    if (pos.y + size.y > windowSize.y)         pos.y = windowSize.y - size.y;

    m_shape.setPosition(pos);
}

void PlayerBase::drawBase(sf::RenderWindow& window)
{
    window.draw(m_shape);
    window.draw(m_hpBar);
    window.draw(m_hpBarFill);

    // Hitbox ataku wręcz widoczny tylko podczas animacji
    if (m_attacking)
        window.draw(m_attackShape);
}

void PlayerBase::checkWallCollisions(
    const std::vector<std::unique_ptr<Wall>>& walls)
{
    for (auto& wall : walls)
    {
        sf::FloatRect playerBounds = m_shape.getGlobalBounds();
        sf::FloatRect wallBounds   = wall->getBounds();

        if (playerBounds.intersects(wallBounds))
        {
            // Obliczamy nakładanie się w każdej osi
            float overlapLeft   = playerBounds.left + playerBounds.width  - wallBounds.left;
            float overlapRight  = wallBounds.left   + wallBounds.width    - playerBounds.left;
            float overlapTop    = playerBounds.top  + playerBounds.height - wallBounds.top;
            float overlapBottom = wallBounds.top    + wallBounds.height   - playerBounds.top;

            float minOverlapX = std::min(overlapLeft,  overlapRight);
            float minOverlapY = std::min(overlapTop,   overlapBottom);

            // Cofamy gracza w kierunku najmniejszego nakładania (AABB resolution)
            if (minOverlapX < minOverlapY)
            {
                if (overlapLeft < overlapRight)
                    m_shape.move(-overlapLeft, 0.f);
                else
                    m_shape.move(overlapRight, 0.f);
            }
            else
            {
                if (overlapTop < overlapBottom)
                    m_shape.move(0.f, -overlapTop);
                else
                    m_shape.move(0.f, overlapBottom);
            }
        }
    }
}

void PlayerBase::onAttackKeyPressed(sf::Keyboard::Key key)
{
    // Ustawiamy flagę tylko jeśli to nasz klawisz ataku
    if (key == m_attackKey)
        m_attackKeyPressed = true;
}

// --- Implementacje getterów ---
sf::FloatRect PlayerBase::getBounds() const    { return m_shape.getGlobalBounds();       }
sf::FloatRect PlayerBase::getAttackBounds() const { return m_attackShape.getGlobalBounds(); }
bool          PlayerBase::isAttacking() const  { return m_attacking;                     }
int           PlayerBase::getHP() const        { return m_hp;                            }
int           PlayerBase::getMaxHP() const     { return m_maxHP;                         }
bool          PlayerBase::isAlive() const      { return m_hp > 0;                        }
bool          PlayerBase::hasDealtHit() const  { return m_hitDealt;                      }
void          PlayerBase::setHitDealt(bool v)  { m_hitDealt = v;                         }
int           PlayerBase::getDamage() const    { return m_damage;                        }

std::vector<std::unique_ptr<Projectile>>& PlayerBase::getProjectiles()
{
    return m_projectiles;
}

void PlayerBase::takeDamage(int amount)
{
    m_hp -= amount;
    if (m_hp < 0) m_hp = 0;
}

void PlayerBase::heal(int amount)
{
    m_hp += amount;
    if (m_hp > m_maxHP)
        m_hp = m_maxHP;
}