#include "Game.h"
#include "Warrior.h"
#include "Mage.h"

// Konstruktor - tu inicjalizujemy wszystkie pola
// Dwukropek po Game() to "lista inicjalizacyjna" -
// inicjalizuje pola PRZED wejściem do ciała konstruktora
Game::Game()
    : m_window(sf::VideoMode(800, 600), "BattleArena")
    , m_state(GameState::Menu)
    , m_player1Choice(0)
    , m_player2Choice(0)
    , m_player1Ready(false)
    , m_player2Ready(false)
{
    // Ładujemy czcionkę systemową
    m_font.loadFromFile("C:/Windows/Fonts/arial.ttf");

    // Tworzymy domyślnych graczy (podmienią się po wyborze)
    m_player1 = createPlayer(0, 100.f, 280.f,
                             sf::Keyboard::W, sf::Keyboard::S,
                             sf::Keyboard::A, sf::Keyboard::D,
                             sf::Keyboard::F);

    m_player2 = createPlayer(0, 660.f, 280.f,
                             sf::Keyboard::Up, sf::Keyboard::Down,
                             sf::Keyboard::Left, sf::Keyboard::Right,
                             sf::Keyboard::Numpad0);
}

std::unique_ptr<PlayerBase> Game::createPlayer(
    int characterIndex,
    float startX, float startY,
    sf::Keyboard::Key upKey,
    sf::Keyboard::Key downKey,
    sf::Keyboard::Key leftKey,
    sf::Keyboard::Key rightKey,
    sf::Keyboard::Key attackKey)
{
    // Tworzymy odpowiednią klasę postaci
    // make_unique tworzy obiekt i zwraca unique_ptr
    if (characterIndex == 0)
        return std::make_unique<Warrior>(startX, startY,
               upKey, downKey, leftKey, rightKey, attackKey);
    else
        return std::make_unique<Mage>(startX, startY,
               upKey, downKey, leftKey, rightKey, attackKey);
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

        if (event.type == sf::Event::KeyPressed)
        {
            // Escape zamyka grę
            if (event.key.code == sf::Keyboard::Escape)
                m_window.close();

            // --- OBSŁUGA MENU ---
            if (m_state == GameState::Menu)
            {
                if (event.key.code == sf::Keyboard::Return)
                    m_state = GameState::CharacterSelect;
            }

            // --- OBSŁUGA WYBORU POSTACI ---
            else if (m_state == GameState::CharacterSelect)
            {
                // Gracz 1 wybiera klawiszami A/D
                if (!m_player1Ready)
                {
                    if (event.key.code == sf::Keyboard::A)
                        m_player1Choice = 0; // Warrior
                    if (event.key.code == sf::Keyboard::D)
                        m_player1Choice = 1; // Mage
                    if (event.key.code == sf::Keyboard::F)
                        m_player1Ready = true;
                }

                // Gracz 2 wybiera strzałkami
                if (!m_player2Ready)
                {
                    if (event.key.code == sf::Keyboard::Left)
                        m_player2Choice = 0; // Warrior
                    if (event.key.code == sf::Keyboard::Right)
                        m_player2Choice = 1; // Mage
                    if (event.key.code == sf::Keyboard::Numpad0)
                        m_player2Ready = true;
                }

                // Obaj gotowi - startujemy!
                if (m_player1Ready && m_player2Ready)
                {
                    // Tworzymy graczy z wybranymi klasami
                    m_player1 = createPlayer(m_player1Choice,
                                 100.f, 280.f,
                                 sf::Keyboard::W, sf::Keyboard::S,
                                 sf::Keyboard::A, sf::Keyboard::D,
                                 sf::Keyboard::F);

                    m_player2 = createPlayer(m_player2Choice,
                                 660.f, 280.f,
                                 sf::Keyboard::Up, sf::Keyboard::Down,
                                 sf::Keyboard::Left, sf::Keyboard::Right,
                                 sf::Keyboard::Numpad0);

                    m_state = GameState::Playing;
                }
            }

            // --- OBSŁUGA GAME OVER ---
            else if (m_state == GameState::GameOver)
            {
                if (event.key.code == sf::Keyboard::Return)
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

    m_player1->update(deltaTime, windowSize);
    m_player2->update(deltaTime, windowSize);

    // Sprawdź czy ktoś kogoś trafił
    checkCombat();

    // Sprawdź czy ktoś przegrał
    if (!m_player1->isAlive() || !m_player2->isAlive())
        m_state = GameState::GameOver;
}

void Game::checkCombat()
{
    // --- ATAK BEZPOŚREDNI (Wojownik) ---

    // Czy gracz 1 trafił gracza 2 atakiem bezpośrednim?
    // m_hitDealt zapewnia że jeden atak = jedne obrażenia
    if (m_player1->isAttacking() &&
        !m_player1->hasDealtHit() &&
        m_player1->getAttackBounds().intersects(m_player2->getBounds()))
    {
        m_player2->takeDamage(m_player1->getDamage());
        m_player1->setHitDealt(true);
    }

    // Czy gracz 2 trafił gracza 1 atakiem bezpośrednim?
    if (m_player2->isAttacking() &&
        !m_player2->hasDealtHit() &&
        m_player2->getAttackBounds().intersects(m_player1->getBounds()))
    {
        m_player1->takeDamage(m_player2->getDamage());
        m_player2->setHitDealt(true);
    }

    // --- POCISKI ---

    // Sprawdź pociski gracza 1 czy trafił gracza 2
    for (auto& projectile : m_player1->getProjectiles())
    {
        if (projectile->isActive() &&
            projectile->getBounds().intersects(m_player2->getBounds()))
        {
            m_player2->takeDamage(projectile->getDamage());
            projectile->deactivate(); // pocisk znika po trafieniu
        }
    }

    // Sprawdź pociski gracza 2 czy trafił gracza 1
    for (auto& projectile : m_player2->getProjectiles())
    {
        if (projectile->isActive() &&
            projectile->getBounds().intersects(m_player1->getBounds()))
        {
            m_player1->takeDamage(projectile->getDamage());
            projectile->deactivate(); // pocisk znika po trafieniu
        }
    }
}

void Game::resetGame()
{
    // Resetujemy wybory i stan
    m_player1Choice = 0;
    m_player2Choice = 0;
    m_player1Ready = false;
    m_player2Ready = false;
    m_state = GameState::CharacterSelect;
}

void Game::render()
{
    // Czyścimy ekran na ciemnoszaro
    m_window.clear(sf::Color(50, 50, 50));

    // Rysujemy różne rzeczy zależnie od stanu gry
    if (m_state == GameState::Menu)
        renderMenu();
    else if (m_state == GameState::CharacterSelect)
        renderCharacterSelect();
    else if (m_state == GameState::Playing)
    {
        // Tu będziemy rysować obiekty gry
        m_player1->draw(m_window);
        m_player2->draw(m_window);
    }
    else if (m_state == GameState::GameOver)
    {
        // Rysujemy graczy w tle żeby było widać końcową pozycję
        m_player1->draw(m_window);
        m_player2->draw(m_window);
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
    title.setPosition((800.f - titleBounds.width) / 2.f, 150.f);

    // Instrukcja sterowania - gracz 1
    sf::Text controls1;
    controls1.setFont(m_font);
    controls1.setString("Gracz 1 (Niebieski): WSAD + F");
    controls1.setCharacterSize(22);
    controls1.setFillColor(sf::Color(100, 100, 255));

    sf::FloatRect c1Bounds = controls1.getLocalBounds();
    controls1.setPosition((800.f - c1Bounds.width) / 2.f, 300.f);

    // Instrukcja sterowania - gracz 2
    sf::Text controls2;
    controls2.setFont(m_font);
    controls2.setString("Gracz 2 (Czerwony): Strzalki + Numpad 0");
    controls2.setCharacterSize(22);
    controls2.setFillColor(sf::Color(255, 100, 100));

    sf::FloatRect c2Bounds = controls2.getLocalBounds();
    controls2.setPosition((800.f - c2Bounds.width) / 2.f, 340.f);

    // Prompt żeby zacząć
    sf::Text startPrompt;
    startPrompt.setFont(m_font);
    startPrompt.setString("Nacisnij ENTER aby zaczac");
    startPrompt.setCharacterSize(28);
    startPrompt.setFillColor(sf::Color::Yellow);

    sf::FloatRect promptBounds = startPrompt.getLocalBounds();
    startPrompt.setPosition((800.f - promptBounds.width) / 2.f, 450.f);

    m_window.draw(title);
    m_window.draw(controls1);
    m_window.draw(controls2);
    m_window.draw(startPrompt);
}

void Game::renderCharacterSelect()
{
    // Tytuł ekranu
    sf::Text title;
    title.setFont(m_font);
    title.setString("Wybierz postac!");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition((800.f - tb.width) / 2.f, 30.f);
    m_window.draw(title);

    // Nazwy klas
    std::string classNames[2] = { "Wojownik", "Mag" };
    sf::Color classColors[2] = {
        sf::Color(50, 100, 255),   // niebieski - wojownik
        sf::Color(180, 50, 255)    // fioletowy - mag
    };
    std::string classStats[2] = {
        "HP: 150 | Spd: 150 | Dmg: 25 | Zasieg: bliski",
        "HP: 80  | Spd: 250 | Dmg: 15 | Zasieg: daleki"
    };

    // Rysujemy karty wyboru dla gracza 1 (lewa strona)
    for (int i = 0; i < 2; i++)
    {
        float x = 50.f + i * 180.f;
        float y = 150.f;

        // Podświetl wybraną klasę
        sf::RectangleShape card(sf::Vector2f(150.f, 200.f));
        card.setPosition(x, y);
        card.setFillColor(m_player1Choice == i ?
            sf::Color(80, 80, 80) : sf::Color(40, 40, 40));
        card.setOutlineThickness(m_player1Choice == i ? 3.f : 1.f);
        card.setOutlineColor(classColors[i]);
        m_window.draw(card);

        // Kolor postaci
        sf::RectangleShape preview(sf::Vector2f(60.f, 60.f));
        preview.setFillColor(classColors[i]);
        preview.setPosition(x + 45.f, y + 20.f);
        m_window.draw(preview);

        // Nazwa klasy
        sf::Text nameText;
        nameText.setFont(m_font);
        nameText.setString(classNames[i]);
        nameText.setCharacterSize(18);
        nameText.setFillColor(classColors[i]);
        nameText.setPosition(x + 10.f, y + 100.f);
        m_window.draw(nameText);

        // Statystyki
        sf::Text statsText;
        statsText.setFont(m_font);
        statsText.setString(classStats[i]);
        statsText.setCharacterSize(10);
        statsText.setFillColor(sf::Color::White);
        statsText.setPosition(x + 5.f, y + 130.f);
        m_window.draw(statsText);
    }

    // Instrukcja gracza 1
    sf::Text p1info;
    p1info.setFont(m_font);
    p1info.setString(m_player1Ready ? "GOTOWY!" : "Gracz 1: A/D = wybor, F = zatwierdz");
    p1info.setCharacterSize(16);
    p1info.setFillColor(m_player1Ready ? sf::Color::Green : sf::Color(100, 100, 255));
    p1info.setPosition(50.f, 370.f);
    m_window.draw(p1info);

    // Rysujemy karty wyboru dla gracza 2 (prawa strona)
    for (int i = 0; i < 2; i++)
    {
        float x = 420.f + i * 180.f;
        float y = 150.f;

        sf::RectangleShape card(sf::Vector2f(150.f, 200.f));
        card.setPosition(x, y);
        card.setFillColor(m_player2Choice == i ?
            sf::Color(80, 80, 80) : sf::Color(40, 40, 40));
        card.setOutlineThickness(m_player2Choice == i ? 3.f : 1.f);
        card.setOutlineColor(classColors[i]);
        m_window.draw(card);

        sf::RectangleShape preview(sf::Vector2f(60.f, 60.f));
        preview.setFillColor(classColors[i]);
        preview.setPosition(x + 45.f, y + 20.f);
        m_window.draw(preview);

        sf::Text nameText;
        nameText.setFont(m_font);
        nameText.setString(classNames[i]);
        nameText.setCharacterSize(18);
        nameText.setFillColor(classColors[i]);
        nameText.setPosition(x + 10.f, y + 100.f);
        m_window.draw(nameText);

        sf::Text statsText;
        statsText.setFont(m_font);
        statsText.setString(classStats[i]);
        statsText.setCharacterSize(10);
        statsText.setFillColor(sf::Color::White);
        statsText.setPosition(x + 5.f, y + 130.f);
        m_window.draw(statsText);
    }

    // Instrukcja gracza 2
    sf::Text p2info;
    p2info.setFont(m_font);
    p2info.setString(m_player2Ready ? "GOTOWY!" : "Gracz 2: </> = wybor, Num0 = zatwierdz");
    p2info.setCharacterSize(16);
    p2info.setFillColor(m_player2Ready ? sf::Color::Green : sf::Color(255, 100, 100));
    p2info.setPosition(420.f, 370.f);
    m_window.draw(p2info);
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

    if (!m_player1->isAlive() && !m_player2->isAlive())
    {
        winnerText = "REMIS!";
        winnerColor = sf::Color::White;
    }
    else if (!m_player1->isAlive())
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
    winner.setPosition((800.f - winnerBounds.width) / 2.f, 200.f);

    // Prompt żeby zagrać jeszcze raz
    sf::Text restartPrompt;
    restartPrompt.setFont(m_font);
    restartPrompt.setString("Nacisnij ENTER aby zagrac jeszcze raz");
    restartPrompt.setCharacterSize(26);
    restartPrompt.setFillColor(sf::Color::Yellow);

    sf::FloatRect restartBounds = restartPrompt.getLocalBounds();
    restartPrompt.setPosition((800.f - restartBounds.width) / 2.f, 350.f);

    m_window.draw(winner);
    m_window.draw(restartPrompt);
}