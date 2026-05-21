#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Direction.h"
#include "Projectile.h"

/**
 * @brief Bazowa klasa gracza - zawiera wspólną logikę dla wszystkich klas postaci
 * 
 * "virtual" przy metodach oznacza że klasy pochodne (Warrior, Mage)
 * mogą je nadpisać swoją własną implementacją.
 */
class PlayerBase
{
public:
    /**
     * @brief Konstruktor gracza
     * @param startX      - startowa pozycja X
     * @param startY      - startowa pozycja Y
     * @param color       - kolor prostokąta gracza
     * @param upKey       - klawisz ruchu w górę
     * @param downKey     - klawisz ruchu w dół
     * @param leftKey     - klawisz ruchu w lewo
     * @param rightKey    - klawisz ruchu w prawo
     * @param attackKey   - klawisz ataku
     * @param hp          - punkty życia
     * @param speed       - prędkość poruszania
     * @param damage      - obrażenia ataku
     * @param attackRange - zasięg ataku w pikselach
     */
    PlayerBase(float startX, float startY,
               sf::Color color,
               sf::Keyboard::Key upKey,
               sf::Keyboard::Key downKey,
               sf::Keyboard::Key leftKey,
               sf::Keyboard::Key rightKey,
               sf::Keyboard::Key attackKey,
               int hp,
               float speed,
               int damage,
               float attackRange);

    // Destruktor wirtualny - konieczny gdy używamy dziedziczenia!
    // Bez tego mogą wystąpić wycieki pamięci
    virtual ~PlayerBase() = default;

    // Aktualizuje logikę gracza (ruch, atak, HP)
    virtual void update(float deltaTime, const sf::Vector2u& windowSize);

    // Rysuje gracza na ekranie
    // virtual = klasy pochodne mogą to nadpisać (np. mag rysuje efekty)
    virtual void draw(sf::RenderWindow& window);

    // Gettery - pozwalają innym klasom odczytać dane gracza
    // ale nie zmieniać ich bezpośrednio (enkapsulacja!)
    sf::FloatRect getBounds() const;       // prostokąt gracza
    sf::FloatRect getAttackBounds() const; // prostokąt ataku
    bool isAttacking() const;              // czy atakuje?
    int getHP() const;                     // ile ma HP?
    int getMaxHP() const;                  // ile ma max HP?
    bool isAlive() const;                  // czy żyje?
    bool hasDealtHit() const;
    void setHitDealt(bool value);
    int getDamage() const;

    // Zwraca wektor aktywnych pocisków tego gracza
    // Game używa tego do sprawdzania kolizji
    std::vector<std::unique_ptr<Projectile>>& getProjectiles();

    // Zadaj obrażenia graczowi
    void takeDamage(int amount);

protected:
    // protected = dostępne dla klas pochodnych (Warrior, Mage)
    // ale nie dla kodu zewnętrznego
    void updateAttack(float deltaTime);
    void clampToWindow(const sf::Vector2u& windowSize);

    sf::RectangleShape m_shape;       // prostokąt gracza
    sf::RectangleShape m_attackShape; // prostokąt ataku (używany przez Warrior)
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
    int m_damage;        // obrażenia ataku
    float m_attackRange; // zasięg ataku w pikselach

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

    // Pociski - każdy gracz może mieć własne (mag używa, wojownik nie)
    // unique_ptr automatycznie zwalnia pamięć gdy pocisk jest usuwany
    std::vector<std::unique_ptr<Projectile>> m_projectiles;
};