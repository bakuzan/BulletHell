#include <cmath>
#include <iostream>

#include "Constants.h"
#include "GameState.h"
#include "InputUtils.h"

GameState::GameState(GameData &data, StateManager &manager, sf::RenderWindow &window)
    : gameData(data), stateManager(manager), window(window)
{
    // Load background
    if (!backgroundTexture.loadFromFile("resources/background.png"))
    {
        throw std::runtime_error("Failed to load background image!");
    }
    backgroundTexture.setRepeated(true);
    background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setTexture(&backgroundTexture);

    // Load player
    if (!spaceshipsTexture.loadFromFile("resources/spaceships_brighter.png"))
    {
        throw std::runtime_error("Failed to load spaceships texture!");
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
        // Maintain the height of the view to match the window height
        float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
        view.setSize(Constants::VIEW_HEIGHT * aspectRatio, Constants::VIEW_HEIGHT);

        // Resize the background to match the new window size
        background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));

        // Adjust texture rect for seamless tiling
        background.setTextureRect(sf::IntRect(
            view.getCenter().x - view.getSize().x / 2.0f,
            view.getCenter().y - view.getSize().y / 2.0f,
            window.getSize().x,
            window.getSize().y));
    }
}

void GameState::update(sf::Time deltaTime, sf::RenderWindow &window)
{
    // Move the player
    const float playerSpeed = Constants::BASE_PLAYER_SPEED;
    if (InputUtils::isAnyKeyPressed({sf::Keyboard::Left, sf::Keyboard::A}))
    {
        player.move(-playerSpeed * deltaTime.asSeconds(), 0);
    }
    if (InputUtils::isAnyKeyPressed({sf::Keyboard::Right, sf::Keyboard::D}))
    {
        player.move(playerSpeed * deltaTime.asSeconds(), 0);
    }
    if (InputUtils::isAnyKeyPressed({sf::Keyboard::Up, sf::Keyboard::W}))
    {
        player.move(0, -playerSpeed * deltaTime.asSeconds());
    }
    if (InputUtils::isAnyKeyPressed({sf::Keyboard::Down, sf::Keyboard::S}))
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
    background.setTextureRect(sf::IntRect(viewPos.x, viewPos.y,
                                          view.getSize().x, view.getSize().y));

    // Update player position in GameData
    gameData.setPlayerPosition(player.getPosition());
}

void GameState::render(sf::RenderWindow &window)
{
    window.setView(view);
    window.draw(background);
    window.draw(player);
}