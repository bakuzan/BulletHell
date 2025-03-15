#include <cmath>
#include <iostream>

#include "Constants.h"
#include "GameState.h"

GameState::GameState(GameData &data, StateManager &manager, sf::RenderWindow &window)
    : gameData(data), stateManager(manager)
{
    // Load background
    if (!backgroundTexture.loadFromFile("resources/background.png"))
    {
        std::cerr << "Failed to load background image!" << std::endl;
    }
    backgroundTexture.setRepeated(true);
    background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setTexture(&backgroundTexture);

    // Load player
    if (!spaceshipsTexture.loadFromFile("resources/spaceships_brighter.png"))
    {
        std::cerr << "Failed to load player texture!" << std::endl;
    }

    player.setTexture(spaceshipsTexture);
    player.setTextureRect(sf::IntRect(3 * Constants::SPRITE_SIZE, 0, Constants::SPRITE_SIZE, Constants::SPRITE_SIZE));
    player.setScale(0.25f, 0.25f);
    player.setOrigin(Constants::SPRITE_SIZE / 2.0f, Constants::SPRITE_SIZE / 2.0f);
    player.setPosition(100.0f, 100.0f);

    // Set up the view
    view.setSize(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT);
    view.setCenter(player.getPosition());
}

GameState::~GameState()
{
    // Destructor
}

// Publics

void GameState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::Resized)
    {
        // Adjust view and background size
        float aspectRatio = float(event.size.width) / float(event.size.height);
        view.setSize(Constants::VIEW_HEIGHT * aspectRatio, Constants::VIEW_HEIGHT);
        background.setSize(sf::Vector2f(event.size.width, event.size.height));
    }
}

void GameState::update(sf::Time deltaTime, sf::RenderWindow &window)
{
    // Move the player
    const float playerSpeed = Constants::BASE_PLAYER_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        player.move(-playerSpeed * deltaTime.asSeconds(), 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        player.move(playerSpeed * deltaTime.asSeconds(), 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        player.move(0, -playerSpeed * deltaTime.asSeconds());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        player.move(0, playerSpeed * deltaTime.asSeconds());
    }

    // Turn the player to face the mouse position
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);
    sf::Vector2f currentPlayerPos = player.getPosition();

    float dx = worldMousePosition.x - currentPlayerPos.x;
    float dy = worldMousePosition.y - currentPlayerPos.y;
    float angle = std::atan2(dy, dx) * 180.f / 3.14159f; // Convert to degrees
    player.setRotation(angle + 270.f);                   // Adjust for sprite alignment

    // Update view to follow player
    view.setCenter(player.getPosition());

    // Update background texture rect for tiling
    sf::Vector2f viewPos = view.getCenter() - view.getSize() / 2.0f;
    background.setPosition(viewPos);
    background.setTextureRect(sf::IntRect(viewPos.x, viewPos.y, view.getSize().x, view.getSize().y));

    // Update player position in GameData
    gameData.setPlayerPosition(player.getPosition());
}

void GameState::render(sf::RenderWindow &window)
{

    // Draw
    window.setView(view);
    window.draw(background);
    window.draw(player);
}