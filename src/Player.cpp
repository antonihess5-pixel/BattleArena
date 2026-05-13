#include "Player.h"

// Konstruktor - inicjalizujemy wszystkie pola
// Dwukropek po Player() to "lista inicjalizacyjna" -
// inicjalizuje pola PRZED wejściem do ciała konstruktora
Player::Player(float startX, float startY,
               sf::Color color,
               sf::Keyboard::Key upKey,
               sf::Keyboard::Key downKey,
               sf::Keyboard::Key leftKey,
               sf::Keyboard::Key rightKey,
               sf::Keyboard::Key attackKey)
    : m_upKey(upKey)
    , m_downKey(downKey)
    , m_leftKey(leftKey)
    , m_rightKey(rightKey)
    , m_attackKey(attackKey)
    , m_speed(200.f)
    , m_hp(100)
    , m_maxHP(100)
    , m_direction(Direction::Right) // domyślnie gracz patrzy w prawo
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

    // Prostokąt ataku (mniejszy, pojawi się obok gracza)
    m_attackShape.setSize(sf::Vector2f(30.f, 30.f));
    m_attackShape.setFillColor(sf::Color(255, 255, 0, 180)); // żółty półprzezroczysty
    m_attackShape.setPosition(-100.f, -100.f); // schowany poza ekranem

    // Pasek HP - tło (ciemnoczerwone)
    m_hpBar.setSize(sf::Vector2f(40.f, 6.f));
    m_hpBar.setFillColor(sf::Color(150, 0, 0));

    // Pasek HP - wypełnienie (kolor zmienia się z HP)
    m_hpBarFill.setSize(sf::Vector2f(40.f, 6.f));
    m_hpBarFill.setFillColor(sf::Color::Green);
}

void Player::update(float deltaTime, const sf::Vector2u& windowSize)
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

void Player::updateAttack(float deltaTime)
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

        // Przesuwamy prostokąt ataku w kierunku w którym patrzy gracz
        // 45.f to odstęp od gracza - tyle żeby atak był tuż obok
        switch (m_direction)
        {
            case Direction::Up:
                // Atak pojawia się nad graczem
                attackPos = { pos.x + 5.f, pos.y - 35.f };
                break;
            case Direction::Down:
                // Atak pojawia się pod graczem
                attackPos = { pos.x + 5.f, pos.y + 45.f };
                break;
            case Direction::Left:
                // Atak pojawia się po lewej stronie
                attackPos = { pos.x - 35.f, pos.y + 5.f };
                break;
            case Direction::Right:
                // Atak pojawia się po prawej stronie
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

void Player::clampToWindow(const sf::Vector2u& windowSize)
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

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
    window.draw(m_hpBar);
    window.draw(m_hpBarFill);

    // Prostokąt ataku rysujemy tylko gdy gracz atakuje
    if (m_attacking)
        window.draw(m_attackShape);
}

// --- Gettery ---
sf::FloatRect Player::getBounds() const
{
    return m_shape.getGlobalBounds();
}

sf::FloatRect Player::getAttackBounds() const
{
    return m_attackShape.getGlobalBounds();
}

bool Player::isAttacking() const
{
    return m_attacking;
}

int Player::getHP() const
{
    return m_hp;
}

bool Player::isAlive() const
{
    return m_hp > 0;
}

void Player::takeDamage(int amount)
{
    m_hp -= amount;
    if (m_hp < 0)
        m_hp = 0;
}

bool Player::hasDealtHit() const
{
    return m_hitDealt;
}

void Player::setHitDealt(bool value)
{
    m_hitDealt = value;
}