#include "PlayerBase.h"

// Konstruktor - inicjalizujemy wszystkie pola
// Dwukropek po PlayerBase() to "lista inicjalizacyjna" -
// inicjalizuje pola PRZED wejściem do ciała konstruktora
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
    , m_speed(speed)             // teraz z parametru
    , m_hp(hp)                   // teraz z parametru
    , m_maxHP(hp)
    , m_damage(damage)           // teraz z parametru
    , m_attackRange(attackRange) // teraz z parametru
    , m_direction(Direction::Right)
    , m_attacking(false)
    , m_attackTimer(0.f)
    , m_attackDuration(0.2f)     // atak trwa 0.2 sekundy
    , m_attackCooldown(0.5f)     // cooldown 0.5 sekundy
    , m_attackCooldownTimer(0.f)
    , m_hitDealt(false)
{
    // Ustawiamy wygląd prostokąta gracza
    m_shape.setSize(sf::Vector2f(40.f, 40.f));  // 40x40 pikseli
    m_shape.setFillColor(color);
    m_shape.setPosition(startX, startY);

    // Prostokąt ataku - rozmiar zależy od zasięgu
    m_attackShape.setSize(sf::Vector2f(attackRange, attackRange * 0.75f));
    m_attackShape.setFillColor(sf::Color(255, 255, 0, 180)); // żółty półprzezroczysty
    m_attackShape.setPosition(-100.f, -100.f); // schowany poza ekranem

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
    // deltaTime to czas który minął od ostatniej klatki
    // Dzięki temu ruch jest płynny niezależnie od FPS
    sf::Vector2f movement(0.f, 0.f);

    // Sprawdzamy czy dany klawisz jest wciśnięty
    // i zapamiętujemy kierunek ostatniego ruchu
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

    // Przesuwamy prostokąt o obliczony wektor ruchu
    m_shape.move(movement);

    // --- GRANICE EKRANU ---
    clampToWindow(windowSize);

    // --- ATAK ---
    updateAttack(deltaTime);

    // --- PASEK HP ---
    sf::Vector2f pos = m_shape.getPosition();

    // Pasek HP nad głową gracza
    m_hpBar.setPosition(pos.x, pos.y - 10.f);
    m_hpBarFill.setPosition(pos.x, pos.y - 10.f);

    // Szerokość paska zależy od aktualnego HP
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

void PlayerBase::updateAttack(float deltaTime)
{
    // Odliczaj cooldown między atakami
    if (m_attackCooldownTimer > 0.f)
        m_attackCooldownTimer -= deltaTime;

    // Wciśnięto klawisz ataku i cooldown minął
    if (sf::Keyboard::isKeyPressed(m_attackKey) &&
        !m_attacking &&
        m_attackCooldownTimer <= 0.f)
    {
        m_attacking = true;
        m_attackTimer = m_attackDuration;
        m_hitDealt = false;
    }

    if (m_attacking)
    {
        m_attackTimer -= deltaTime;

        // Pozycja ataku zależy od kierunku gracza
        sf::Vector2f pos = m_shape.getPosition();
        sf::Vector2f attackPos;
        float range = m_attackShape.getSize().x;

        // Przesuwamy prostokąt ataku w kierunku w którym patrzy gracz
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

        // Atak się skończył
        if (m_attackTimer <= 0.f)
        {
            m_attacking = false;
            m_attackCooldownTimer = m_attackCooldown;
            // Schowaj prostokąt ataku poza ekranem
            m_attackShape.setPosition(-100.f, -100.f);
        }
    }
}

void PlayerBase::clampToWindow(const sf::Vector2u& windowSize)
{
    sf::Vector2f pos = m_shape.getPosition();
    sf::Vector2f size = m_shape.getSize();

    // Nie wychodź poza lewą i prawą krawędź
    if (pos.x < 0.f)
        pos.x = 0.f;
    if (pos.x + size.x > windowSize.x)
        pos.x = windowSize.x - size.x;

    // Nie wychodź poza górną i dolną krawędź
    if (pos.y < 0.f)
        pos.y = 0.f;
    if (pos.y + size.y > windowSize.y)
        pos.y = windowSize.y - size.y;

    m_shape.setPosition(pos);
}

void PlayerBase::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
    window.draw(m_hpBar);
    window.draw(m_hpBarFill);

    // Prostokąt ataku rysujemy tylko gdy gracz atakuje
    if (m_attacking)
        window.draw(m_attackShape);
}

std::vector<std::unique_ptr<Projectile>>& PlayerBase::getProjectiles()
{
    return m_projectiles;
}

// --- Gettery ---
sf::FloatRect PlayerBase::getBounds() const
{
    return m_shape.getGlobalBounds();
}

sf::FloatRect PlayerBase::getAttackBounds() const
{
    return m_attackShape.getGlobalBounds();
}

bool PlayerBase::isAttacking() const { return m_attacking; }
int PlayerBase::getHP() const { return m_hp; }
int PlayerBase::getMaxHP() const { return m_maxHP; }
bool PlayerBase::isAlive() const { return m_hp > 0; }
bool PlayerBase::hasDealtHit() const { return m_hitDealt; }
void PlayerBase::setHitDealt(bool value) { m_hitDealt = value; }
int PlayerBase::getDamage() const { return m_damage; }

void PlayerBase::takeDamage(int amount)
{
    m_hp -= amount;
    if (m_hp < 0)
        m_hp = 0;
}