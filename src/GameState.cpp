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

    // Load projectiles
    if (!projectileTexture.loadFromFile("resources/projectiles.png"))
    {
        throw std::runtime_error("Failed to load projectiles texture!");
    }

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

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        shootProjectile = true;
    }

    if (InputUtils::isAnyKeyPressed({sf::Keyboard::Space}))
    {
        shootProjectile = true;
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
    float angleRadians = std::atan2(dy, dx);
    float angle = angleRadians * (180.f / 3.14159f); // Convert to degrees
    player.setRotation(angle + 270.f);               // Adjust for sprite alignment

    // Did the player shoot? Create projectile(s)
    if (shootProjectile)
    {
        // Calculate the bullet's velocity based on the player's rotation
        sf::Vector2f direction(std::cos(angleRadians), std::sin(angleRadians));
        sf::Vector2f bulletVelocity = direction * (Constants::PROJECTILE_SPEED_BULLET);

        // Calculate bullet spawn position (front-center of the player)
        float playerScale = player.getScale().x;                                          // Assume it is the same both x/y
        float bulletWidth = 25.0f;                                                        // (size is the sprite size (500) * sprite scale (0.05))
        float bulletHeight = 25.0f;                                                       // (size is the sprite size (500) * sprite scale (0.05))
        sf::Vector2f offset = direction * ((Constants::SPRITE_SIZE * playerScale) / 2.f); // Move to front
        sf::Vector2f rotatedCenterOffset = sf::Vector2f(
            -bulletWidth / 2.f * std::cos(angleRadians) + bulletHeight / 2.f * std::sin(angleRadians),
            -bulletWidth / 2.f * std::sin(angleRadians) - bulletHeight / 2.f * std::cos(angleRadians));

        // Calculate final spawn position
        sf::Vector2f spawnPosition = player.getPosition() + offset + rotatedCenterOffset;

        auto &projectiles = gameData.getProjectiles();
        const auto &textureRect = gameData.projectileTextureManager.getTextureRect(ProjectileType::BULLET);
        projectiles.emplace_back(spawnPosition.x, spawnPosition.y,
                                 projectileTexture, textureRect,
                                 bulletVelocity, ProjectileType::BULLET);

        shootProjectile = false;
    }

    // Update projectiles
    auto &projectiles = gameData.getProjectiles();
    for (auto it = projectiles.begin(); it != projectiles.end();)
    {
        it->update(deltaTime);

        if (it->isOffScreen(window))
        {
            it = projectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Update view to follow player
    view.setCenter(player.getPosition());

    // Update background texture rect for tiling
    sf::Vector2f viewPos = view.getCenter() - view.getSize() / 2.0f;
    background.setPosition(viewPos);
    background.setTextureRect(sf::IntRect(viewPos.x, viewPos.y,
                                          view.getSize().x, view.getSize().y));
}

void GameState::render(sf::RenderWindow &window)
{
    window.setView(view);
    window.draw(background);

    auto &projectiles = gameData.getProjectiles();
    for (const auto &projectile : projectiles)
    {
        projectile.render(window);
    }

    window.draw(player);
}