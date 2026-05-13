#include "Game.h"

// Konstruktor - tu inicjalizujemy wszystkie pola
// Dwukropek po Game() to "lista inicjalizacyjna" -
// inicjalizuje pola PRZED wejściem do ciała konstruktora
Game::Game()
    : m_window(sf::VideoMode(800, 600), "BattleArena")
    // Gracz 1: niebieski, sterowanie WSAD + F (atak)
    , m_player1(100.f, 280.f,
                sf::Color::Blue,
                sf::Keyboard::W,
                sf::Keyboard::S,
                sf::Keyboard::A,
                sf::Keyboard::D,
                sf::Keyboard::F)
    // Gracz 2: czerwony, strzałki + numpad 0 (atak)
    , m_player2(660.f, 280.f,
                sf::Color::Red,
                sf::Keyboard::Up,
                sf::Keyboard::Down,
                sf::Keyboard::Left,
                sf::Keyboard::Right,
                sf::Keyboard::Numpad0)
    // Zaczynamy od ekranu menu
    , m_state(GameState::Menu)
{
    // Ładujemy czcionkę systemową
    // Arial jest dostępny na każdym Windowsie
    m_font.loadFromFile("C:/Windows/Fonts/arial.ttf");
}

void Game::run()
{
    // Pętla gry - działa dopóki okno jest otwarte
    while (m_window.isOpen())
    {
        // deltaTime - czas między klatkami w sekundach
        float deltaTime = m_clock.restart().asSeconds();

        processEvents();
        update(deltaTime);
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

        // Escape zamyka grę
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
                m_window.close();

            // Enter - przejście między stanami
            if (event.key.code == sf::Keyboard::Return)
            {
                if (m_state == GameState::Menu)
                    m_state = GameState::Playing;

                else if (m_state == GameState::GameOver)
                    resetGame();
            }
        }
    }
}

void Game::update(float deltaTime)
{
    // Aktualizujemy logikę tylko podczas rozgrywki
    if (m_state != GameState::Playing)
        return;

    sf::Vector2u windowSize = m_window.getSize();

    m_player1.update(deltaTime, windowSize);
    m_player2.update(deltaTime, windowSize);

    // Sprawdź czy ktoś kogoś trafił
    checkCombat();

    // Sprawdź czy ktoś przegrał
    if (!m_player1.isAlive() || !m_player2.isAlive())
        m_state = GameState::GameOver;
}

void Game::checkCombat()
{
    // Czy gracz 1 trafił gracza 2?
    // m_hitDealt zapewnia że jeden atak = jedne obrażenia
    if (m_player1.isAttacking() &&
        !m_player1.hasDealtHit() &&
        m_player1.getAttackBounds().intersects(m_player2.getBounds()))
    {
        m_player2.takeDamage(10);
        m_player1.setHitDealt(true); // zaznacz że cios już zadany
    }

    // Czy gracz 2 trafił gracza 1?
    if (m_player2.isAttacking() &&
        !m_player2.hasDealtHit() &&
        m_player2.getAttackBounds().intersects(m_player1.getBounds()))
    {
        m_player1.takeDamage(10);
        m_player2.setHitDealt(true); // zaznacz że cios już zadany
    }
}

void Game::resetGame()
{
    // Tworzymy graczy od nowa z początkowymi wartościami
    m_player1 = Player(100.f, 280.f,
                       sf::Color::Blue,
                       sf::Keyboard::W,
                       sf::Keyboard::S,
                       sf::Keyboard::A,
                       sf::Keyboard::D,
                       sf::Keyboard::F);

    m_player2 = Player(660.f, 280.f,
                       sf::Color::Red,
                       sf::Keyboard::Up,
                       sf::Keyboard::Down,
                       sf::Keyboard::Left,
                       sf::Keyboard::Right,
                       sf::Keyboard::Numpad0);

    m_state = GameState::Playing;
}

void Game::render()
{
    // Czyścimy ekran na ciemnoszaro
    m_window.clear(sf::Color(50, 50, 50));

    // Rysujemy różne rzeczy zależnie od stanu gry
    if (m_state == GameState::Menu)
    {
        renderMenu();
    }
    else if (m_state == GameState::Playing)
    {
        // Tu będziemy rysować obiekty gry
        m_player1.draw(m_window);
        m_player2.draw(m_window);
    }
    else if (m_state == GameState::GameOver)
    {
        // Rysujemy graczy w tle żeby było widać końcową pozycję
        m_player1.draw(m_window);
        m_player2.draw(m_window);
        renderGameOver();
    }

    // Wyświetlamy to co narysowaliśmy
    m_window.display();
}

void Game::renderMenu()
{
    // Tytuł gry
    sf::Text title;
    title.setFont(m_font);
    title.setString("BATTLE ARENA");
    title.setCharacterSize(64);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);

    // Centrujemy tekst na ekranie
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(
        (800.f - titleBounds.width) / 2.f,
        150.f
    );

    // Instrukcja sterowania - gracz 1
    sf::Text controls1;
    controls1.setFont(m_font);
    controls1.setString("Gracz 1 (Niebieski): WSAD + F");
    controls1.setCharacterSize(22);
    controls1.setFillColor(sf::Color(100, 100, 255)); // niebieskawa

    sf::FloatRect c1Bounds = controls1.getLocalBounds();
    controls1.setPosition((800.f - c1Bounds.width) / 2.f, 300.f);

    // Instrukcja sterowania - gracz 2
    sf::Text controls2;
    controls2.setFont(m_font);
    controls2.setString("Gracz 2 (Czerwony): Strzalki + Numpad 0");
    controls2.setCharacterSize(22);
    controls2.setFillColor(sf::Color(255, 100, 100)); // czerwonawa

    sf::FloatRect c2Bounds = controls2.getLocalBounds();
    controls2.setPosition((800.f - c2Bounds.width) / 2.f, 340.f);

    // Prompt żeby zacząć
    sf::Text startPrompt;
    startPrompt.setFont(m_font);
    startPrompt.setString("Nacisnij ENTER aby zaczac");
    startPrompt.setCharacterSize(28);
    startPrompt.setFillColor(sf::Color::Yellow);

    sf::FloatRect promptBounds = startPrompt.getLocalBounds();
    startPrompt.setPosition(
        (800.f - promptBounds.width) / 2.f,
        450.f
    );

    m_window.draw(title);
    m_window.draw(controls1);
    m_window.draw(controls2);
    m_window.draw(startPrompt);
}

void Game::renderGameOver()
{
    // Przyciemnione tło żeby tekst był czytelny
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(800.f, 600.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 150)); // czarne półprzezroczyste
    m_window.draw(overlay);

    // Ustal kto wygrał
    std::string winnerText;
    sf::Color winnerColor;

    if (!m_player1.isAlive() && !m_player2.isAlive())
    {
        // Obaj zginęli w tej samej klatce - remis
        winnerText = "REMIS!";
        winnerColor = sf::Color::White;
    }
    else if (!m_player1.isAlive())
    {
        winnerText = "Gracz 2 wygrywa!";
        winnerColor = sf::Color::Red;
    }
    else
    {
        winnerText = "Gracz 1 wygrywa!";
        winnerColor = sf::Color::Blue;
    }

    // Tekst zwycięzcy
    sf::Text winner;
    winner.setFont(m_font);
    winner.setString(winnerText);
    winner.setCharacterSize(56);
    winner.setFillColor(winnerColor);
    winner.setStyle(sf::Text::Bold);

    sf::FloatRect winnerBounds = winner.getLocalBounds();
    winner.setPosition(
        (800.f - winnerBounds.width) / 2.f,
        200.f
    );

    // Prompt żeby zagrać jeszcze raz
    sf::Text restartPrompt;
    restartPrompt.setFont(m_font);
    restartPrompt.setString("Nacisnij ENTER aby zagrac jeszcze raz");
    restartPrompt.setCharacterSize(26);
    restartPrompt.setFillColor(sf::Color::Yellow);

    sf::FloatRect restartBounds = restartPrompt.getLocalBounds();
    restartPrompt.setPosition(
        (800.f - restartBounds.width) / 2.f,
        350.f
    );

    m_window.draw(winner);
    m_window.draw(restartPrompt);
}