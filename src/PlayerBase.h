/**
 * @file PlayerBase.h
 * @brief Deklaracja abstrakcyjnej klasy bazowej PlayerBase
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Direction.h"
#include "Projectile.h"

class Wall;

/**
 * @brief Abstrakcyjna klasa bazowa dla wszystkich postaci gracza
 *
 * Zawiera wspólną logikę dla klas Warrior, Mage i Archer:
 * - Ruch po ekranie z uwzględnieniem deltaTime
 * - Ograniczenie ruchu do granic okna (clamp)
 * - System punktów życia (HP) z paskiem zdrowia
 * - Abstrakcyjna metoda draw() wymuszająca własny wygląd w klasach pochodnych
 * - Obsługa kolizji ze ścianami
 * - Przekazywanie klawisza ataku przez event system
 *
 * Klasa jest abstrakcyjna - nie można stworzyć jej obiektu bezpośrednio.
 * Należy użyć jednej z klas pochodnych: Warrior, Mage lub Archer.
 */
class PlayerBase
{
public:
    /**
     * @brief Konstruktor klasy bazowej gracza
     * @param startX      Startowa pozycja X w pikselach
     * @param startY      Startowa pozycja Y w pikselach
     * @param color       Kolor prostokąta reprezentującego gracza
     * @param upKey       Klawisz ruchu w górę
     * @param downKey     Klawisz ruchu w dół
     * @param leftKey     Klawisz ruchu w lewo
     * @param rightKey    Klawisz ruchu w prawo
     * @param attackKey   Klawisz ataku
     * @param hp          Startowe i maksymalne punkty życia
     * @param speed       Prędkość poruszania w pikselach na sekundę
     * @param damage      Obrażenia zadawane przy ataku
     * @param attackRange Zasięg ataku wręcz w pikselach (0 dla klas strzelających)
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

    /**
     * @brief Wirtualny destruktor
     *
     * Wymagany przy dziedziczeniu - bez niego destruktory klas pochodnych
     * nie byłyby wywoływane przy usuwaniu przez wskaźnik do klasy bazowej,
     * co powoduje wycieki pamięci.
     */
    virtual ~PlayerBase() = default;

    /**
     * @brief Aktualizuje logikę gracza
     *
     * Obsługuje ruch, ograniczenie do ekranu, atak wręcz i pasek HP.
     * Klasy pochodne (Mage, Archer) nadpisują tę metodę żeby obsłużyć pociski.
     * @param deltaTime  Czas od ostatniej klatki w sekundach
     * @param windowSize Rozmiar okna gry do ograniczenia ruchu
     */
    virtual void update(float deltaTime, const sf::Vector2u& windowSize);

    /**
     * @brief Rysuje gracza na ekranie
     *
     * Czysto wirtualna - każda klasa postaci rysuje się inaczej.
     * Nie można stworzyć obiektu PlayerBase bo ta metoda nie ma implementacji.
     * @param window Referencja do okna gry
     */
    virtual void draw(sf::RenderWindow& window) = 0;

    /**
     * @brief Zwraca prostokąt kolizji gracza
     * @return Prostokąt w przestrzeni świata (po transformacjach)
     */
    sf::FloatRect getBounds() const;

    /**
     * @brief Zwraca prostokąt kolizji ataku wręcz
     *
     * Zwraca aktywny hitbox tylko podczas ataku Warriora.
     * Dla Maga i Archera hitbox jest schowany poza ekranem.
     * @return Prostokąt hitboxa ataku
     */
    sf::FloatRect getAttackBounds() const;

    /**
     * @brief Sprawdza czy gracz aktualnie wykonuje atak
     * @return true jeśli animacja ataku jest aktywna
     */
    bool isAttacking() const;

    /**
     * @brief Zwraca aktualne punkty życia
     * @return Aktualne HP (0..maxHP)
     */
    int getHP() const;

    /**
     * @brief Zwraca maksymalne punkty życia
     * @return Maksymalne HP ustawione przy tworzeniu gracza
     */
    int getMaxHP() const;

    /**
     * @brief Sprawdza czy gracz żyje
     * @return true jeśli HP > 0
     */
    bool isAlive() const;

    /**
     * @brief Sprawdza czy atak już zadał obrażenia w tej animacji
     * @return true jeśli obrażenia zostały już zadane w bieżącym ataku
     */
    bool hasDealtHit() const;

    /**
     * @brief Ustawia flagę zadania obrażeń
     *
     * Wywoływane przez Game::checkCombat() po zadaniu obrażeń,
     * zapobiega wielokrotnemu trafieniu w jednej animacji ataku.
     * @param value Nowa wartość flagi
     */
    void setHitDealt(bool value);

    /**
     * @brief Zwraca obrażenia zadawane przez tego gracza
     * @return Wartość obrażeń przy ataku
     */
    int getDamage() const;

    /**
     * @brief Zwraca referencję do wektora pocisków gracza
     *
     * Używane przez Game::checkCombat() do sprawdzania kolizji
     * pocisków z przeciwnikiem. Warrior zwraca pusty wektor.
     * @return Referencja do wektora aktywnych pocisków
     */
    std::vector<std::unique_ptr<Projectile>>& getProjectiles();

    /**
     * @brief Zadaje obrażenia graczowi
     *
     * Zmniejsza HP o podaną wartość. HP nie może spaść poniżej 0.
     * @param amount Liczba obrażeń do zadania
     */
    void takeDamage(int amount);

    /**
     * @brief Sprawdza kolizję gracza ze ścianami i cofa go
     *
     * Używa algorytmu AABB (Axis-Aligned Bounding Box) do wykrycia
     * nakładania się prostokątów i cofnięcia gracza o minimalną odległość.
     * @param walls Wektor ścian na planszy
     */
    void checkWallCollisions(const std::vector<std::unique_ptr<Wall>>& walls);

    /**
     * @brief Informuje gracza o wciśnięciu klawisza
     *
     * Wywoływane przez Game::processEvents() dla każdego zdarzenia klawiatury.
     * Gracz sam sprawdza czy wciśnięty klawisz to jego klawisz ataku.
     * Podejście event-based zapobiega problemom z powtarzaniem klawiszy.
     * @param key Kod wciśniętego klawisza SFML
     */
    void onAttackKeyPressed(sf::Keyboard::Key key);

    /**
     * @brief Leczy gracza o podaną wartość
     *
     * HP nie może przekroczyć maksymalnej wartości.
     * @param amount Liczba punktów życia do przywrócenia
     */
    void heal(int amount);  

protected:
    /**
     * @brief Aktualizuje logikę ataku wręcz (Warrior)
     *
     * Obsługuje cooldown, animację ataku i pozycjonowanie hitboxa.
     * Klasy strzelające (Mage, Archer) nie wywołują tej metody.
     * @param deltaTime Czas od ostatniej klatki w sekundach
     */
    void updateAttack(float deltaTime);

    /**
     * @brief Ogranicza pozycję gracza do granic okna
     * @param windowSize Rozmiar okna gry
     */
    void clampToWindow(const sf::Vector2u& windowSize);

    /**
     * @brief Rysuje wspólne elementy wszystkich postaci
     *
     * Rysuje: kształt gracza, pasek HP (tło i wypełnienie),
     * hitbox ataku wręcz (jeśli aktywny).
     * Klasy pochodne wywołują tę metodę w swojej implementacji draw().
     * @param window Referencja do okna gry
     */
    void drawBase(sf::RenderWindow& window);

    sf::RectangleShape m_shape;      ///< Prostokąt reprezentujący gracza
    sf::RectangleShape m_attackShape;///< Hitbox ataku wręcz (Warrior)
    sf::RectangleShape m_hpBar;      ///< Pasek HP - tło (ciemnoczerwone)
    sf::RectangleShape m_hpBarFill;  ///< Pasek HP - wypełnienie (zielony/żółty/czerwony)

    sf::Keyboard::Key m_upKey;    ///< Klawisz ruchu w górę
    sf::Keyboard::Key m_downKey;  ///< Klawisz ruchu w dół
    sf::Keyboard::Key m_leftKey;  ///< Klawisz ruchu w lewo
    sf::Keyboard::Key m_rightKey; ///< Klawisz ruchu w prawo
    sf::Keyboard::Key m_attackKey;///< Klawisz ataku

    float m_speed;       ///< Prędkość ruchu w pikselach na sekundę
    int   m_hp;          ///< Aktualne punkty życia
    int   m_maxHP;       ///< Maksymalne punkty życia
    int   m_damage;      ///< Obrażenia zadawane przy ataku
    float m_attackRange; ///< Zasięg ataku wręcz w pikselach

    Direction m_direction; ///< Ostatni kierunek ruchu - określa kierunek ataku

    bool  m_attacking;           ///< Czy animacja ataku jest aktywna
    float m_attackTimer;         ///< Pozostały czas animacji ataku
    float m_attackDuration;      ///< Całkowity czas trwania animacji ataku
    float m_attackCooldown;      ///< Czas odnowienia między atakami
    float m_attackCooldownTimer; ///< Pozostały czas cooldownu
    bool  m_hitDealt;            ///< Czy obrażenia zostały zadane w tym ataku
    bool  m_attackKeyPressed;    ///< Flaga wciśnięcia klawisza ataku (event-based)

    std::vector<std::unique_ptr<Projectile>> m_projectiles; ///< Aktywne pociski gracza
};