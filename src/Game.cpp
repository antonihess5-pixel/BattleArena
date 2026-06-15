/**
 * @file Game.cpp
 * @brief Implementacja klasy Game
 */

#include "Game.h"
#include "Warrior.h"
#include "Mage.h"
#include "Archer.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

Game::Game()
    : m_window(sf::VideoMode::getDesktopMode(),
               "BattleArena",
               sf::Style::Fullscreen)
    , m_state(GameState::Menu)
    , m_playerChoices(2, 0)
    , m_playerReady(2, false)
    , m_healthPackTimer(0.f)
    , m_healthPackInterval(8.f)
{
    // Ładujemy czcionkę systemową dostępną na każdym Windowsie
    m_font.loadFromFile("C:/Windows/Fonts/arial.ttf");

    // Tworzymy domyślnych graczy - zostaną podmienieni po wyborze postaci
    m_players.push_back(createPlayer(0,
        m_window.getSize().x * 0.35f,
        m_window.getSize().y * 0.5f,
        sf::Keyboard::W, sf::Keyboard::S,
        sf::Keyboard::A, sf::Keyboard::D,
        sf::Keyboard::F));

    m_players.push_back(createPlayer(0,
        m_window.getSize().x * 0.65f,
        m_window.getSize().y * 0.5f,
        sf::Keyboard::Up, sf::Keyboard::Down,
        sf::Keyboard::Left, sf::Keyboard::Right,
        sf::Keyboard::RControl));
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
    // Fabryka postaci - polimorfizm przez wskaźnik do klasy bazowej
    if (characterIndex == 0)
        return std::make_unique<Warrior>(
            startX, startY,
            upKey, downKey, leftKey, rightKey, attackKey);
    else if (characterIndex == 1)
        return std::make_unique<Mage>(
            startX, startY,
            upKey, downKey, leftKey, rightKey, attackKey);
    else
        return std::make_unique<Archer>(
            startX, startY,
            upKey, downKey, leftKey, rightKey, attackKey);
    srand(static_cast<unsigned>(time(nullptr)));
}

void Game::createBorderWalls()
{
    float W = m_window.getSize().x;
    float H = m_window.getSize().y;

    // Parametry areny identyczne jak w WallPlacer
    float arenaSize = std::min(W, H) * 0.75f;
    float cellSize  = arenaSize / 16.f;
    float offsetX   = (W - arenaSize) / 2.f;
    float offsetY   = (H - arenaSize) / 2.f;

    // Górna i dolna krawędź areny (16 ścian poziomych każda)
    for (int i = 0; i < 16; i++)
    {
        m_walls.push_back(std::make_unique<Wall>(
            i, 0, i + 1, 0, cellSize, offsetX, offsetY));
        m_walls.push_back(std::make_unique<Wall>(
            i, 16, i + 1, 16, cellSize, offsetX, offsetY));
    }

    // Lewa i prawa krawędź areny (16 ścian pionowych każda)
    for (int i = 0; i < 16; i++)
    {
        m_walls.push_back(std::make_unique<Wall>(
            0, i, 0, i + 1, cellSize, offsetX, offsetY));
        m_walls.push_back(std::make_unique<Wall>(
            16, i, 16, i + 1, cellSize, offsetX, offsetY));
    }
}

void Game::run()
{
    // Główna pętla gry - działa dopóki okno jest otwarte
    while (m_window.isOpen())
    {
        // Mierzymy czas między klatkami dla płynnej animacji
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
            // Escape zawsze zamyka grę niezależnie od stanu
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
                // Gracz 1: A/D = wybór, F = zatwierdź
                if (!m_playerReady[0])
                {
                    if (event.key.code == sf::Keyboard::A)
                        // (choice+2)%3 = cofnięcie bez ujemnych indeksów
                        m_playerChoices[0] = (m_playerChoices[0] + 2) % 3;
                    if (event.key.code == sf::Keyboard::D)
                        m_playerChoices[0] = (m_playerChoices[0] + 1) % 3;
                    if (event.key.code == sf::Keyboard::F)
                        m_playerReady[0] = true;
                }

                // Gracz 2: strzałki = wybór, Numpad0 = zatwierdź
                if (!m_playerReady[1])
                {
                    if (event.key.code == sf::Keyboard::Left)
                        m_playerChoices[1] = (m_playerChoices[1] + 2) % 3;
                    if (event.key.code == sf::Keyboard::Right)
                        m_playerChoices[1] = (m_playerChoices[1] + 1) % 3;
                    if (event.key.code == sf::Keyboard::Numpad0)
                        m_playerReady[1] = true;
                }

                // Obaj gotowi - tworzymy graczy i przechodzimy do układania ścian
                if (m_playerReady[0] && m_playerReady[1])
                {
                    m_walls.clear();
                    createBorderWalls();

                    m_players.clear();
                    m_players.push_back(createPlayer(
                        m_playerChoices[0],
                        m_window.getSize().x * 0.35f,
                        m_window.getSize().y * 0.5f,
                        sf::Keyboard::W, sf::Keyboard::S,
                        sf::Keyboard::A, sf::Keyboard::D,
                        sf::Keyboard::F));

                    m_players.push_back(createPlayer(
                        m_playerChoices[1],
                        m_window.getSize().x * 0.65f,
                        m_window.getSize().y * 0.5f,
                        sf::Keyboard::Up, sf::Keyboard::Down,
                        sf::Keyboard::Left, sf::Keyboard::Right,
                        sf::Keyboard::RControl));

                    m_wallPlacer = std::make_unique<WallPlacer>(
                        10,  // ilość ścian do ułożenia
                        m_window.getSize().x,
                        m_window.getSize().y);

                    m_state = GameState::WallPlacement;
                }
            }

            // --- OBSŁUGA UKŁADANIA ŚCIAN ---
            else if (m_state == GameState::WallPlacement)
            {
                // handleInput zwraca true gdy obaj gracze skończyli
                if (m_wallPlacer->handleInput(event.key.code))
                {
                    // Przenosimy ściany z WallPlacer zachowując ściany brzegowe
                    for (auto& wall : m_wallPlacer->getWalls())
                        m_walls.push_back(std::make_unique<Wall>(*wall));

                    m_wallPlacer.reset(); // zwalniamy WallPlacer
                    m_state = GameState::Playing;
                }
            }

            // --- OBSŁUGA GAME OVER ---
            else if (m_state == GameState::GameOver)
            {
                if (event.key.code == sf::Keyboard::Return)
                    resetGame();
            }

            // Przekazujemy wciśnięcie klawisza do graczy (atak event-based)
            if (m_state == GameState::Playing)
            {
                for (auto& player : m_players)
                    player->onAttackKeyPressed(event.key.code);
            }
        }
    }
}

void Game::update(float deltaTime)
{
    if (m_state != GameState::Playing)
        return;

    sf::Vector2u windowSize = m_window.getSize();

    for (auto& player : m_players)
        player->update(deltaTime, windowSize);

    for (auto& player : m_players)
        player->checkWallCollisions(m_walls);

    for (auto& player : m_players)
        for (auto& projectile : player->getProjectiles())
            projectile->checkWallCollisions(m_walls);

    checkCombat();

    // Timer spawnu apteczek
    m_healthPackTimer += deltaTime;
    if (m_healthPackTimer >= m_healthPackInterval &&
        m_healthPacks.size() < 3) // max 3 apteczki na raz
    {
        spawnHealthPack();
        m_healthPackTimer = 0.f;
    }

    updateHealthPacks();

    for (auto& player : m_players)
    {
        if (!player->isAlive())
        {
            m_state = GameState::GameOver;
            return;
        }
    }
}

void Game::checkCombat()
{
    // Sprawdzamy każdą parę graczy (i atakuje j)
    for (int i = 0; i < (int)m_players.size(); i++)
    {
        for (int j = 0; j < (int)m_players.size(); j++)
        {
            if (i == j) continue; // gracz nie może trafić sam siebie

            // --- ATAK WRĘCZ (Warrior) ---
            if (m_players[i]->isAttacking() &&
                !m_players[i]->hasDealtHit() &&
                m_players[i]->getAttackBounds()
                    .intersects(m_players[j]->getBounds()))
            {
                m_players[j]->takeDamage(m_players[i]->getDamage());
                m_players[i]->setHitDealt(true); // jeden atak = jedne obrażenia
            }

            // --- POCISKI (Mage i Archer) ---
            // getProjectiles() zwraca pusty wektor dla Warriora
            for (auto& projectile : m_players[i]->getProjectiles())
            {
                if (projectile->isActive() &&
                    projectile->getBounds()
                        .intersects(m_players[j]->getBounds()))
                {
                    m_players[j]->takeDamage(projectile->getDamage());
                    projectile->deactivate(); // pocisk znika po trafieniu
                }
            }
        }
    }
}

void Game::resetGame()
{
    std::fill(m_playerChoices.begin(), m_playerChoices.end(), 0);
    std::fill(m_playerReady.begin(),   m_playerReady.end(),   false);
    m_walls.clear();
    m_healthPacks.clear(); // wyczyść apteczki przy restarcie
    m_healthPackTimer = 0.f;
    m_state = GameState::CharacterSelect;
}

void Game::render()
{
    // Ciemnoszare tło areny
    m_window.clear(sf::Color(50, 50, 50));

    if (m_state == GameState::Menu)
        renderMenu();
    else if (m_state == GameState::CharacterSelect)
        renderCharacterSelect();
    else if (m_state == GameState::WallPlacement)
    {
        // Podczas układania ścian rysujemy ściany brzegowe w tle
        for (auto& wall : m_walls)
            wall->draw(m_window);
        m_wallPlacer->draw(m_window, m_font);
    }
    else if (m_state == GameState::Playing)
    {
        for (auto& wall : m_walls)
            wall->draw(m_window);

        // Rysuj apteczki pod graczami
        for (auto& pack : m_healthPacks)
            pack.draw(m_window);

        for (auto& player : m_players)
            player->draw(m_window);
    }
    else if (m_state == GameState::GameOver)
    {
        for (auto& wall : m_walls)
            wall->draw(m_window);
        for (auto& player : m_players)
            player->draw(m_window);
        renderGameOver();
    }

    m_window.display();
}

void Game::renderMenu()
{
    float W = m_window.getSize().x;
    float H = m_window.getSize().y;

    sf::Text title;
    title.setFont(m_font);
    title.setString("BATTLE ARENA");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition((W - tb.width) / 2.f, H * 0.25f);
    m_window.draw(title);

    sf::Text controls1;
    controls1.setFont(m_font);
    controls1.setString("Gracz 1: WSAD + F (atak)");
    controls1.setCharacterSize(26);
    controls1.setFillColor(sf::Color(100, 100, 255));
    sf::FloatRect c1b = controls1.getLocalBounds();
    controls1.setPosition((W - c1b.width) / 2.f, H * 0.5f);
    m_window.draw(controls1);

    sf::Text controls2;
    controls2.setFont(m_font);
    controls2.setString("Gracz 2: Strzalki + Prawy Ctrl (atak)");
    controls2.setCharacterSize(26);
    controls2.setFillColor(sf::Color(255, 100, 100));
    sf::FloatRect c2b = controls2.getLocalBounds();
    controls2.setPosition((W - c2b.width) / 2.f, H * 0.57f);
    m_window.draw(controls2);

    sf::Text prompt;
    prompt.setFont(m_font);
    prompt.setString("Nacisnij ENTER aby zaczac");
    prompt.setCharacterSize(32);
    prompt.setFillColor(sf::Color::Yellow);
    sf::FloatRect pb = prompt.getLocalBounds();
    prompt.setPosition((W - pb.width) / 2.f, H * 0.72f);
    m_window.draw(prompt);
}

void Game::renderCharacterSelect()
{
    float W = m_window.getSize().x;
    float H = m_window.getSize().y;

    sf::Text title;
    title.setFont(m_font);
    title.setString("Wybierz postac!");
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition((W - tb.width) / 2.f, H * 0.05f);
    m_window.draw(title);

    std::string classNames[3] = { "Wojownik", "Mag", "Lucznik" };
    sf::Color classColors[3] = {
        sf::Color(50, 100, 255),
        sf::Color(180, 50, 255),
        sf::Color(50, 200, 80)
    };
    std::string classStats[3] = {
        "HP: 150\nSpd: 150\nDmg: 25\nZasieg: bliski",
        "HP: 80\nSpd: 250\nDmg: 15\nZasieg: daleki",
        "HP: 110\nSpd: 200\nDmg: 20\nZasieg: sredni"
    };

    float cardW       = 180.f;
    float cardH       = 280.f;
    float cardSpacing = 220.f;

    // --- KARTY GRACZA 1 (lewa strona ekranu) ---
    float p1StartX = W * 0.05f;
    float cardsY   = H * 0.2f;

    for (int i = 0; i < 3; i++)
    {
        float x = p1StartX + i * cardSpacing;

        sf::RectangleShape card(sf::Vector2f(cardW, cardH));
        card.setPosition(x, cardsY);
        card.setFillColor(m_playerChoices[0] == i ?
            sf::Color(80, 80, 80) : sf::Color(30, 30, 30));
        card.setOutlineThickness(m_playerChoices[0] == i ? 4.f : 1.f);
        card.setOutlineColor(classColors[i]);
        m_window.draw(card);

        sf::RectangleShape preview(sf::Vector2f(80.f, 80.f));
        preview.setFillColor(classColors[i]);
        preview.setPosition(x + (cardW - 80.f) / 2.f, cardsY + 20.f);
        m_window.draw(preview);

        sf::Text nameText;
        nameText.setFont(m_font);
        nameText.setString(classNames[i]);
        nameText.setCharacterSize(22);
        nameText.setFillColor(classColors[i]);
        nameText.setStyle(sf::Text::Bold);
        sf::FloatRect nb = nameText.getLocalBounds();
        nameText.setPosition(x + (cardW - nb.width) / 2.f, cardsY + 115.f);
        m_window.draw(nameText);

        sf::Text statsText;
        statsText.setFont(m_font);
        statsText.setString(classStats[i]);
        statsText.setCharacterSize(16);
        statsText.setFillColor(sf::Color(200, 200, 200));
        statsText.setPosition(x + 15.f, cardsY + 150.f);
        m_window.draw(statsText);
    }

    sf::Text p1label;
    p1label.setFont(m_font);
    p1label.setString("GRACZ 1");
    p1label.setCharacterSize(28);
    p1label.setFillColor(sf::Color(100, 100, 255));
    p1label.setStyle(sf::Text::Bold);
    p1label.setPosition(p1StartX, H * 0.1f);
    m_window.draw(p1label);

    sf::Text p1info;
    p1info.setFont(m_font);
    p1info.setString(m_playerReady[0] ?
        "GOTOWY!" : "A/D = wybor, F = zatwierdz");
    p1info.setCharacterSize(20);
    p1info.setFillColor(m_playerReady[0] ?
        sf::Color::Green : sf::Color(100, 100, 255));
    p1info.setPosition(p1StartX, cardsY + cardH + 20.f);
    m_window.draw(p1info);

    // --- KARTY GRACZA 2 (prawa strona ekranu) ---
    float p2StartX = W * 0.52f;

    for (int i = 0; i < 3; i++)
    {
        float x = p2StartX + i * cardSpacing;

        sf::RectangleShape card(sf::Vector2f(cardW, cardH));
        card.setPosition(x, cardsY);
        card.setFillColor(m_playerChoices[1] == i ?
            sf::Color(80, 80, 80) : sf::Color(30, 30, 30));
        card.setOutlineThickness(m_playerChoices[1] == i ? 4.f : 1.f);
        card.setOutlineColor(classColors[i]);
        m_window.draw(card);

        sf::RectangleShape preview(sf::Vector2f(80.f, 80.f));
        preview.setFillColor(classColors[i]);
        preview.setPosition(x + (cardW - 80.f) / 2.f, cardsY + 20.f);
        m_window.draw(preview);

        sf::Text nameText;
        nameText.setFont(m_font);
        nameText.setString(classNames[i]);
        nameText.setCharacterSize(22);
        nameText.setFillColor(classColors[i]);
        nameText.setStyle(sf::Text::Bold);
        sf::FloatRect nb = nameText.getLocalBounds();
        nameText.setPosition(x + (cardW - nb.width) / 2.f, cardsY + 115.f);
        m_window.draw(nameText);

        sf::Text statsText;
        statsText.setFont(m_font);
        statsText.setString(classStats[i]);
        statsText.setCharacterSize(16);
        statsText.setFillColor(sf::Color(200, 200, 200));
        statsText.setPosition(x + 15.f, cardsY + 150.f);
        m_window.draw(statsText);
    }

    sf::Text p2label;
    p2label.setFont(m_font);
    p2label.setString("GRACZ 2");
    p2label.setCharacterSize(28);
    p2label.setFillColor(sf::Color(255, 100, 100));
    p2label.setStyle(sf::Text::Bold);
    p2label.setPosition(p2StartX, H * 0.1f);
    m_window.draw(p2label);

    sf::Text p2info;
    p2info.setFont(m_font);
    p2info.setString(m_playerReady[1] ?
        "GOTOWY!" : "</> = wybor, Num0 = zatwierdz");
    p2info.setCharacterSize(20);
    p2info.setFillColor(m_playerReady[1] ?
        sf::Color::Green : sf::Color(255, 100, 100));
    p2info.setPosition(p2StartX, cardsY + cardH + 20.f);
    m_window.draw(p2info);

    // Pionowa linia podziału między sekcjami graczy
    sf::RectangleShape divider(sf::Vector2f(4.f, H * 0.7f));
    divider.setFillColor(sf::Color(80, 80, 80));
    divider.setPosition(W / 2.f - 2.f, H * 0.1f);
    m_window.draw(divider);
}

void Game::renderGameOver()
{
    float W = m_window.getSize().x;
    float H = m_window.getSize().y;

    // Półprzezroczyste czarne tło poprawia czytelność tekstu
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(W, H));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    m_window.draw(overlay);

    // Ustalamy wynik na podstawie HP graczy
    std::string winnerText;
    sf::Color   winnerColor;

    bool p1alive = m_players[0]->isAlive();
    bool p2alive = m_players[1]->isAlive();

    if (!p1alive && !p2alive)
    {
        winnerText  = "REMIS!";
        winnerColor = sf::Color::White;
    }
    else if (!p1alive)
    {
        winnerText  = "Gracz 2 wygrywa!";
        winnerColor = sf::Color::Red;
    }
    else
    {
        winnerText  = "Gracz 1 wygrywa!";
        winnerColor = sf::Color::Blue;
    }

    sf::Text winner;
    winner.setFont(m_font);
    winner.setString(winnerText);
    winner.setCharacterSize(72);
    winner.setFillColor(winnerColor);
    winner.setStyle(sf::Text::Bold);
    sf::FloatRect wb = winner.getLocalBounds();
    winner.setPosition((W - wb.width) / 2.f, H * 0.3f);
    m_window.draw(winner);

    sf::Text prompt;
    prompt.setFont(m_font);
    prompt.setString("Nacisnij ENTER aby zagrac jeszcze raz");
    prompt.setCharacterSize(32);
    prompt.setFillColor(sf::Color::Yellow);
    sf::FloatRect pb = prompt.getLocalBounds();
    prompt.setPosition((W - pb.width) / 2.f, H * 0.55f);
    m_window.draw(prompt);
}

void Game::spawnHealthPack()
{
    float W = m_window.getSize().x;
    float H = m_window.getSize().y;
    float arenaSize = std::min(W, H) * 0.75f;
    float offsetX   = (W - arenaSize) / 2.f;
    float offsetY   = (H - arenaSize) / 2.f;

    // Losujemy pozycję wewnątrz areny z marginesem od ścian
    float margin = 40.f;
    float x = offsetX + margin +
        static_cast<float>(rand()) / RAND_MAX * (arenaSize - 2.f * margin);
    float y = offsetY + margin +
        static_cast<float>(rand()) / RAND_MAX * (arenaSize - 2.f * margin);

    m_healthPacks.emplace_back(x, y, 30);
}

void Game::updateHealthPacks()
{
    // Sprawdź kolizje każdej apteczki z każdym graczem
    for (auto& pack : m_healthPacks)
    {
        if (!pack.isActive()) continue;

        for (auto& player : m_players)
        {
            if (pack.getBounds().intersects(player->getBounds()))
            {
                // Gracz podnosi apteczkę - leczy się ale nie więcej niż maxHP
                int newHP = player->getHP() + pack.getHealing();
                if (newHP > player->getMaxHP())
                    newHP = player->getMaxHP();

                // Musimy dodać metodę heal() do PlayerBase
                player->heal(newHP - player->getHP());
                pack.collect();
                break; // apteczka może być podniesiona tylko raz
            }
        }
    }

    // Usuń nieaktywne apteczki
    m_healthPacks.erase(
        std::remove_if(m_healthPacks.begin(), m_healthPacks.end(),
            [](const HealthPack& p) { return !p.isActive(); }),
        m_healthPacks.end()
    );
}