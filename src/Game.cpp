#include "Game.h"

Game::Game()
    : m_window(sf::VideoMode(800, 600), "BattleArena")
    // Gracz 1: niebieski, sterowanie WSAD
    , m_player1(100.f, 280.f,
                sf::Color::Blue,
                sf::Keyboard::W,
                sf::Keyboard::S,
                sf::Keyboard::A,
                sf::Keyboard::D)
    // Gracz 2: czerwony, sterowanie strzałkami
    , m_player2(660.f, 280.f,
                sf::Color::Red,
                sf::Keyboard::Up,
                sf::Keyboard::Down,
                sf::Keyboard::Left,
                sf::Keyboard::Right)
{
}

void Game::run()
{
    while (m_window.isOpen())
    {
        // deltaTime - czas między klatkami w sekundach
        float deltaTime = m_clock.restart().asSeconds();

        processEvents();
        update(deltaTime);  // przekazujemy deltaTime
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();

        if (event.type == sf::Event::KeyPressed)
            if (event.key.code == sf::Keyboard::Escape)
                m_window.close();
    }
}

void Game::update(float deltaTime)  // teraz przyjmuje deltaTime!
{
    m_player1.update(deltaTime);
    m_player2.update(deltaTime);
}

void Game::render()
{
    m_window.clear(sf::Color(50, 50, 50));  // ciemnoszare tło

    m_player1.draw(m_window);
    m_player2.draw(m_window);

    m_window.display();
}