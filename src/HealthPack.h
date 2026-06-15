/**
 * @file HealthPack.h
 * @brief Deklaracja klasy HealthPack reprezentującej apteczkę na planszy
 */

#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Apteczka pojawiająca się losowo na planszy
 *
 * Gracz który wejdzie na apteczkę odzyskuje określoną ilość HP.
 * Apteczka znika po podniesieniu. Wizualnie reprezentowana
 * jako zielony krzyżyk na planszy.
 */
class HealthPack
{
public:
    /**
     * @brief Konstruktor apteczki
     * @param x       Pozycja X w pikselach
     * @param y       Pozycja Y w pikselach
     * @param healing Ile HP przywraca apteczka
     */
    HealthPack(float x, float y, int healing = 30);

    /**
     * @brief Rysuje apteczkę na ekranie
     * @param window Referencja do okna gry
     */
    void draw(sf::RenderWindow& window) const;

    /**
     * @brief Zwraca prostokąt kolizji apteczki
     * @return Prostokąt do detekcji kolizji z graczem
     */
    sf::FloatRect getBounds() const;

    /**
     * @brief Sprawdza czy apteczka jest aktywna
     * @return true jeśli apteczka nie została jeszcze podniesiona
     */
    bool isActive() const;

    /**
     * @brief Dezaktywuje apteczkę po podniesieniu przez gracza
     */
    void collect();

    /**
     * @brief Zwraca ile HP przywraca apteczka
     * @return Wartość leczenia
     */
    int getHealing() const;

private:
    sf::RectangleShape m_horizontal; ///< Pozioma belka krzyżyka
    sf::RectangleShape m_vertical;   ///< Pionowa belka krzyżyka
    sf::RectangleShape m_background; ///< Tło apteczki
    bool m_active;                   ///< Czy apteczka jest aktywna
    int  m_healing;                  ///< Ile HP przywraca
};