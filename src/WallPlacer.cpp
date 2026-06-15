/**
 * @file WallPlacer.cpp
 * @brief Implementacja klasy WallPlacer
 */

#include "WallPlacer.h"
#include <algorithm>

// Definicja statycznej stałej - wymagana w .cpp oprócz deklaracji w .h
const int WallPlacer::GRID_SIZE;

WallPlacer::WallPlacer(int wallsPerPlayer, float windowWidth, float windowHeight)
    : m_wallsPerPlayer(wallsPerPlayer)
    , m_wallsPlaced(0)
    , m_currentPlayer(0)
    , m_cursorX(GRID_SIZE / 2)
    , m_cursorY(GRID_SIZE / 2)
{
    // Arena zajmuje 75% mniejszego wymiaru okna - zapewnia proporcjonalność
    float arenaSize = std::min(windowWidth, windowHeight) * 0.75f;
    m_cellSize = arenaSize / GRID_SIZE;

    // Centrujemy arenę dokładnie na środku ekranu
    m_offsetX = (windowWidth  - arenaSize) / 2.f;
    m_offsetY = (windowHeight - arenaSize) / 2.f;
}

bool WallPlacer::handleInput(sf::Keyboard::Key key)
{
    // Ruch kursora po węzłach siatki klawiszami WSAD
    if (key == sf::Keyboard::W)
        m_cursorY = std::max(0, m_cursorY - 1);
    if (key == sf::Keyboard::S)
        m_cursorY = std::min(GRID_SIZE, m_cursorY + 1);
    if (key == sf::Keyboard::A)
        m_cursorX = std::max(0, m_cursorX - 1);
    if (key == sf::Keyboard::D)
        m_cursorX = std::min(GRID_SIZE, m_cursorX + 1);

    // Stawianie ścian strzałkami - kierunek = położenie ściany względem kursora
    if (key == sf::Keyboard::Up)    placeWall( 0, -1);
    if (key == sf::Keyboard::Down)  placeWall( 0,  1);
    if (key == sf::Keyboard::Left)  placeWall(-1,  0);
    if (key == sf::Keyboard::Right) placeWall( 1,  0);

    // Faza kończy się gdy obaj gracze postawili wszystkie ściany
    return m_wallsPlaced >= m_wallsPerPlayer * 2;
}

void WallPlacer::placeWall(int dx, int dy)
{
    int nx = m_cursorX + dx; // węzeł docelowy
    int ny = m_cursorY + dy;

    if (!isValidNode(nx, ny))
        return;

    // Nie pozwalamy postawić drugiej ściany w tym samym miejscu
    for (auto& wall : m_walls)
        if (wall->isSamePosition(m_cursorX, m_cursorY, nx, ny))
            return;

    // Tworzymy ścianę z uwzględnieniem offsetu siatki
    m_walls.push_back(std::make_unique<Wall>(
        m_cursorX, m_cursorY, nx, ny,
        m_cellSize, m_offsetX, m_offsetY));

    m_wallsPlaced++;

    // Zmiana tury: 0→1 lub 1→0 za pomocą operacji XOR
    m_currentPlayer = 1 - m_currentPlayer;
}

bool WallPlacer::isValidNode(int x, int y) const
{
    // Siatka 16x16 ma węzły od 0 do 16 włącznie (17x17 węzłów)
    return x >= 0 && x <= GRID_SIZE && y >= 0 && y <= GRID_SIZE;
}

void WallPlacer::draw(sf::RenderWindow& window, sf::Font& font) const
{
    // --- RYSOWANIE SIATKI ---
    for (int i = 0; i <= GRID_SIZE; i++)
    {
        // Linie poziome siatki
        sf::RectangleShape hLine(sf::Vector2f(GRID_SIZE * m_cellSize, 1.f));
        hLine.setPosition(m_offsetX, m_offsetY + i * m_cellSize);
        hLine.setFillColor(sf::Color(80, 80, 80));
        window.draw(hLine);

        // Linie pionowe siatki
        sf::RectangleShape vLine(sf::Vector2f(1.f, GRID_SIZE * m_cellSize));
        vLine.setPosition(m_offsetX + i * m_cellSize, m_offsetY);
        vLine.setFillColor(sf::Color(80, 80, 80));
        window.draw(vLine);
    }

    // --- RYSOWANIE POSTAWIONYCH ŚCIAN ---
    for (auto& wall : m_walls)
        wall->draw(window);

    // --- RYSOWANIE KURSORA ---
    float cx = m_offsetX + m_cursorX * m_cellSize;
    float cy = m_offsetY + m_cursorY * m_cellSize;
    float cursorSize = 12.f;

    sf::CircleShape cursor(cursorSize);
    cursor.setOrigin(cursorSize, cursorSize);
    cursor.setPosition(cx, cy);
    cursor.setFillColor(m_currentPlayer == 0 ?
        sf::Color(100, 100, 255, 200) :  // niebieski - gracz 1
        sf::Color(255, 100, 100, 200));   // czerwony - gracz 2
    cursor.setOutlineThickness(2.f);
    cursor.setOutlineColor(sf::Color::White);
    window.draw(cursor);

    // --- INFORMACJE UI ---
    sf::Text turnText;
    turnText.setFont(font);
    turnText.setString(m_currentPlayer == 0 ?
        "Tura Gracza 1 - WSAD = ruch, Strzalki = postaw sciane" :
        "Tura Gracza 2 - WSAD = ruch, Strzalki = postaw sciane");
    turnText.setCharacterSize(22);
    turnText.setFillColor(m_currentPlayer == 0 ?
        sf::Color(100, 100, 255) : sf::Color(255, 100, 100));
    turnText.setPosition(m_offsetX, m_offsetY - 60.f);
    window.draw(turnText);

    // Licznik postawionych ścian dla każdego gracza
    sf::Text wallCount;
    wallCount.setFont(font);
    wallCount.setString(
        "Gracz 1: " + std::to_string(m_wallsPlaced / 2) +
        "/" + std::to_string(m_wallsPerPlayer) + " scian    " +
        "Gracz 2: " + std::to_string((m_wallsPlaced + 1) / 2) +
        "/" + std::to_string(m_wallsPerPlayer) + " scian"
    );
    wallCount.setCharacterSize(22);
    wallCount.setFillColor(sf::Color::White);
    wallCount.setPosition(m_offsetX, m_offsetY - 30.f);
    window.draw(wallCount);
}

const std::vector<std::unique_ptr<Wall>>& WallPlacer::getWalls() const
{
    return m_walls;
}