#pragma once

#include <SFML/Graphics.hpp>
#include "Direction.h"

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
     * @param attackKey - klawisz ataku
     */
    Player(float startX, float startY,
           sf::Color color,
           sf::Keyboard::Key upKey,
           sf::Keyboard::Key downKey,
           sf::Keyboard::Key leftKey,
           sf::Keyboard::Key rightKey,
           sf::Keyboard::Key attackKey);

    // Aktualizuje logikę gracza (ruch, atak, HP)
    void update(float deltaTime, const sf::Vector2u& windowSize);

    // Rysuje gracza na ekranie
    void draw(sf::RenderWindow& window);

    // Gettery - pozwalają innym klasom odczytać dane gracza
    // ale nie zmieniać ich bezpośrednio (enkapsulacja!)
    sf::FloatRect getBounds() const;       // prostokąt gracza
    sf::FloatRect getAttackBounds() const; // prostokąt ataku
    bool isAttacking() const;              // czy atakuje?
    int getHP() const;                     // ile ma HP?
    bool isAlive() const;                  // czy żyje?

    // Zadaj obrażenia graczowi
    void takeDamage(int amount);

    // Zarządzanie flagą trafienia - jeden atak = jedne obrażenia
    bool hasDealtHit() const;
    void setHitDealt(bool value);

private:
    void updateAttack(float deltaTime);
    void clampToWindow(const sf::Vector2u& windowSize);

    sf::RectangleShape m_shape;       // prostokąt gracza
    sf::RectangleShape m_attackShape; // prostokąt ataku
    sf::RectangleShape m_hpBar;       // pasek HP (tło)
    sf::RectangleShape m_hpBarFill;   // pasek HP (wypełnienie)

    // Klawisze sterowania - każdy gracz ma inne
    sf::Keyboard::Key m_upKey;
    sf::Keyboard::Key m_downKey;
    sf::Keyboard::Key m_leftKey;
    sf::Keyboard::Key m_rightKey;
    sf::Keyboard::Key m_attackKey;

    float m_speed;

    // HP
    int m_hp;
    int m_maxHP;

    // Kierunek w którym patrzy gracz
    // Zapamiętujemy go żeby wiedzieć gdzie pojawi się atak
    Direction m_direction;

    // Atak
    bool m_attacking;            // czy właśnie atakuje
    float m_attackTimer;         // jak długo trwa atak
    float m_attackDuration;      // maksymalny czas ataku
    float m_attackCooldown;      // czas odnowienia ataku
    float m_attackCooldownTimer; // licznik odnowienia
    bool m_hitDealt;             // czy już zadał obrażenia w tym ataku
};