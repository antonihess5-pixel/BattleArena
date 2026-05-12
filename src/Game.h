#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"  // dołączamy klasę Player

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow m_window;
    sf::Clock m_clock;    // zegar do mierzenia deltaTime

    Player m_player1;     // gracz 1
    Player m_player2;     // gracz 2
};