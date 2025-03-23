#include <iostream>
#include <random>

#include "BasicEnemy.h"
#include "Constants.h"
#include "GameState.h"
#include "GameUtils.h"
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
    player.setTextureRect(sf::IntRect(392, Constants::SPRITE_OFFSET_Y, Constants::SPRITE_WIDTH_PLAYER, Constants::SPRITE_HEIGHT_PLAYER));
    player.setScale(0.25f, 0.25f);
    player.setOrigin(Constants::SPRITE_WIDTH_PLAYER / 2.0f, Constants::SPRITE_HEIGHT_PLAYER / 2.0f);
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
    spawnEnemies(deltaTime.asSeconds(), spaceshipsTexture);
    updateEnemies(deltaTime.asSeconds(), player.getPosition());

    movePlayer(deltaTime);
    aimAndShoot(window);

    updateProjectiles(deltaTime, window);

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

    auto &enemies = gameData.getEnemies();
    for (const auto &enemy : enemies)
    {
        enemy->render(window);
    }
}

// Private

void GameState::movePlayer(sf::Time &deltaTime)
{
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
}

void GameState::aimAndShoot(sf::RenderWindow &window)
{
    // Turn the player to face the mouse position
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);
    sf::Vector2f currentPlayerPos = player.getPosition();

    float angleRadians = GameUtils::rotateTowards(
        player,
        currentPlayerPos,
        worldMousePosition,
        270.0f);

    // Did the player shoot? Create projectile(s)
    if (shootProjectile)
    {
        // Calculate the bullet's velocity based on the player's rotation
        sf::Vector2f direction(std::cos(angleRadians), std::sin(angleRadians));
        sf::Vector2f bulletVelocity = direction * (Constants::PROJECTILE_SPEED_BULLET);

        // Calculate bullet spawn position (front-center of the player)
        float playerScale = player.getScale().x;                                                  // Assume it is the same both x/y
        float bulletWidth = 25.0f;                                                                // (size is the sprite size (500) * sprite scale (0.05))
        float bulletHeight = 25.0f;                                                               // (size is the sprite size (500) * sprite scale (0.05))
        sf::Vector2f offset = direction * ((Constants::SPRITE_WIDTH_PLAYER * playerScale) / 2.f); // Move to front
        sf::Vector2f rotatedCenterOffset = sf::Vector2f(
            -bulletWidth / 2.f * std::cos(angleRadians) + bulletHeight / 2.f * std::sin(angleRadians),
            -bulletWidth / 2.f * std::sin(angleRadians) - bulletHeight / 2.f * std::cos(angleRadians));

        // Calculate final spawn position
        sf::Vector2f spawnPosition = player.getPosition() + offset + rotatedCenterOffset;

        auto &projectiles = gameData.getProjectiles();
        const auto &textureRect = gameData.projectileTextureManager.getTextureRect(ProjectileType::BULLET);
        projectiles.emplace_back(ProjectileType::BULLET,
                                 projectileTexture, textureRect,
                                 spawnPosition, bulletVelocity);

        shootProjectile = false;
    }
}

void GameState::updateProjectiles(const sf::Time &deltaTime, sf::RenderWindow &window)
{
    auto &projectiles = gameData.getProjectiles();
    auto &enemies = gameData.getEnemies();

    for (auto projIt = projectiles.begin(); projIt != projectiles.end();)
    {
        projIt->update(deltaTime);

        bool projectileRemoved = false;

        // Check collision with enemies
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();)
        {
            if (projIt->getSprite().getGlobalBounds().intersects((*enemyIt)->getSprite().getGlobalBounds()))
            {
                // TODO add points handling
                enemyIt = enemies.erase(enemyIt);
                projIt = projectiles.erase(projIt);
                projectileRemoved = true;
                break;
            }
            else
            {
                ++enemyIt;
            }
        }

        if (!projectileRemoved)
        {
            if (projIt->isOffScreen(window))
            {
                projIt = projectiles.erase(projIt);
            }
            else
            {
                ++projIt;
            }
        }
    }
}

std::unique_ptr<Enemy> GameState::spawnEnemy(
    EnemyType type,
    const sf::Texture &texture,
    sf::IntRect textureRect,
    sf::Vector2f spawnPosition,
    float speed)
{
    switch (type)
    {
    case EnemyType::BASIC:
        return std::make_unique<BasicEnemy>(texture, textureRect, spawnPosition, speed);
    default:
        return nullptr;
    }
}

void GameState::spawnEnemies(float deltaTime, const sf::Texture &enemiesTexture)
{
    static float spawnAccumulator = 0.0f;

    // Calculate number of enemies to spawn this frame
    spawnAccumulator += Constants::ENEMY_SPAWN_RATE * deltaTime;
    int enemiesToSpawn = static_cast<int>(spawnAccumulator);
    spawnAccumulator -= enemiesToSpawn; // Track fractional spawns

    const float enemySpeed = Constants::BASE_PLAYER_SPEED * 0.33f;
    const auto &textureRect = gameData.enemyTextureManager.getTextureRect(EnemyType::BASIC);
    sf::Vector2f playerPos = player.getPosition();

    for (int i = 0; i < enemiesToSpawn; ++i)
    {
        sf::Vector2f spawnPosition = getRandomSpawnPosition(playerPos, window);
        gameData.addEnemy(
            spawnEnemy(EnemyType::BASIC,
                       enemiesTexture, textureRect,
                       spawnPosition, enemySpeed));
    }
}

void GameState::updateEnemies(float deltaTime, sf::Vector2f playerPosition)
{
    auto &enemies = gameData.getEnemies();
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        (*it)->update(deltaTime, playerPosition);

        if ((*it)->getType() == EnemyType::BASIC &&
            (*it)->getSprite().getGlobalBounds().intersects(player.getGlobalBounds()))
        {
            //  TODO Handle damage logic here
            it = enemies.erase(it); // Enemy has suicided, so remove after collision
        }
        else
        {
            ++it;
        }
    }
}

sf::Vector2f GameState::getRandomSpawnPosition(
    const sf::Vector2f &playerPosition,
    const sf::RenderWindow &window)
{
    sf::Vector2u windowSize = window.getSize();
    int side = rand() % 4; // Choose a random side around the player

    switch (side)
    {
    case 0: // Top
        return {playerPosition.x + static_cast<float>(rand() % windowSize.x) - (windowSize.x / 2.f),
                playerPosition.y - (windowSize.y / 2.f) - Constants::ENEMY_SPAWN_OFFSET};
    case 1: // Left
        return {playerPosition.x - (windowSize.x / 2.f) - Constants::ENEMY_SPAWN_OFFSET,
                playerPosition.y + static_cast<float>(rand() % windowSize.y) - (windowSize.y / 2.f)};
    case 2: // Right
        return {playerPosition.x + (windowSize.x / 2.f) + Constants::ENEMY_SPAWN_OFFSET,
                playerPosition.y + static_cast<float>(rand() % windowSize.y) - (windowSize.y / 2.f)};
    case 3: // Bottom
        return {playerPosition.x + static_cast<float>(rand() % windowSize.x) - (windowSize.x / 2.f),
                playerPosition.y + (windowSize.y / 2.f) + Constants::ENEMY_SPAWN_OFFSET};
    default:
        return {0.f, 0.f}; // Fallback (this should never occur)
    }
}
