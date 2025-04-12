#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#include "entities/BasicEnemy.h"
#include "entities/ShooterEnemy.h"
#include "utils/GameUtils.h"
#include "utils/InputUtils.h"
#include "Constants.h"
#include "GameState.h"
#include "PauseState.h"
#include "GameOverState.h"

GameState::GameState(GameData &data, StateManager &manager, sf::RenderWindow &window)
    : gameData(data),
      stateManager(manager),
      window(window),
      healthBar(gameData.textureManager.getTexture(TextureId::HEALTHBAR_BORDER),
                gameData.textureManager.getTexture(TextureId::HEALTHBAR_FILLING),
                100.0f),
      shootProjectile(false)
{
    // Load background
    const sf::Texture &backgroundTexture = gameData.textureManager.getTexture(TextureId::BACKGROUND);
    background.setTexture(&backgroundTexture);

    // Load player
    const sf::Texture &spaceshipsTexture = gameData.textureManager.getTexture(TextureId::SPACESHIPS);
    player.setTexture(spaceshipsTexture);
    player.setTextureRect(sf::IntRect(392, Constants::SPRITE_OFFSET_Y, Constants::SPRITE_WIDTH_PLAYER, Constants::SPRITE_HEIGHT_PLAYER));
    player.setScale(0.3f, 0.3f);
    player.setOrigin(Constants::SPRITE_WIDTH_PLAYER / 2.0f, Constants::SPRITE_HEIGHT_PLAYER / 2.0f);
    player.setPosition(100.0f, 100.0f);

    // Setup points display
    scoreText.setFont(gameData.gameFont);
    scoreText.setString(std::string(8, '0'));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(10, 50);

    // Set up the view + adjust background
    view.setSize(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT);
    view.setCenter(player.getPosition());
    ensureBackgroundSizeIsLinkedToViewSize(
        view.getCenter() - view.getSize() / 2.0f,
        view.getSize());
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
        ensureBackgroundSizeIsLinkedToViewSize(
            view.getCenter() - view.getSize() / 2.0f,
            view.getSize());
    }

    if (event.type == sf::Event::KeyPressed &&
        InputUtils::isAnyKeyPressed(event.key.code, {sf::Keyboard::Escape, sf::Keyboard::P}))
    {
        stateManager.pushState(std::make_unique<PauseState>(gameData, stateManager, window));
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
    enemySpawnManager.spawnEnemies(
        deltaTime.asSeconds(),
        gameData.getEnemies(),
        gameData.textureManager.getTexture(TextureId::SPACESHIPS),
        player.getPosition(),
        view);

    updateEnemies(deltaTime.asSeconds(), player.getPosition());
    processEnemyShooting(deltaTime.asSeconds(), player.getPosition());

    movePlayer(deltaTime);
    aimAndShoot(window);

    updateProjectiles(deltaTime, window);

    // Update view to follow player
    view.setCenter(player.getPosition());

    // Update background texture rect for tiling by matching the view
    sf::Vector2f viewPos = view.getCenter() - view.getSize() / 2.0f;
    ensureBackgroundSizeIsLinkedToViewSize(
        viewPos,
        view.getSize());
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

    healthBar.render(window, view);
    renderScoreText(window, view);
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
        float bulletWidth = 15.0f;                                                                // (size is the sprite size (300) * sprite scale (0.05))
        float bulletHeight = 25.0f;                                                               // (size is the sprite size (500) * sprite scale (0.05))
        sf::Vector2f offset = direction * ((Constants::SPRITE_WIDTH_PLAYER * playerScale) / 2.f); // Move to front
        sf::Vector2f rotatedCenterOffset = sf::Vector2f(
            -bulletWidth / 2.f * std::cos(angleRadians) + bulletHeight / 2.f * std::sin(angleRadians),
            -bulletWidth / 2.f * std::sin(angleRadians) - bulletHeight / 2.f * std::cos(angleRadians));

        // Calculate final spawn position
        sf::Vector2f spawnPosition = player.getPosition() + offset + rotatedCenterOffset;

        auto &projectiles = gameData.getProjectiles();
        projectiles.emplace_back(ProjectileType::BULLET,
                                 gameData.textureManager.getTexture(TextureId::PROJECTILES),
                                 gameData.projectileTextureManager.getTextureRect(ProjectileType::BULLET),
                                 spawnPosition,
                                 bulletVelocity,
                                 Constants::PROJECTILE_DAMAGE_BULLET);

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

        if (projIt->getOrigin() == ProjectileOrigin::PLAYER)
        {
            for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();)
            {
                if (projIt->getSprite().getGlobalBounds().intersects((*enemyIt)->getSprite().getGlobalBounds()))
                {
                    // Effects of shooting enemy
                    gameData.updateScore((*enemyIt)->getPointsValue());
                    updateScoreText(gameData.getScore());

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
        }
        else if (projIt->getOrigin() == ProjectileOrigin::ENEMY &&
                 projIt->getSprite().getGlobalBounds().intersects(player.getGlobalBounds()))
        {
            projIt = projectiles.erase(projIt);
            projectileRemoved = true;

            // Effects of player being hit
            gameData.updatePlayerHealth(-projIt->getDamageInflicts());
            healthBar.setHealth(gameData.getPlayerHealth());

            if (gameData.getPlayerHealth() <= 0)
            {
                stateManager.pushState(std::make_unique<GameOverState>(gameData, stateManager, window));
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

void GameState::updateEnemies(float deltaTime, const sf::Vector2f &playerPosition)
{
    auto &enemies = gameData.getEnemies();
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        (*it)->update(deltaTime, playerPosition);

        if ((*it)->getType() == EnemyType::BASIC &&
            (*it)->getSprite().getGlobalBounds().intersects(player.getGlobalBounds()))
        {
            it = enemies.erase(it); // Enemy has suicided, so remove after collision

            // Effects of hitting player
            gameData.updatePlayerHealth(-10);
            healthBar.setHealth(gameData.getPlayerHealth());

            if (gameData.getPlayerHealth() <= 0)
            {
                stateManager.pushState(std::make_unique<GameOverState>(gameData, stateManager, window));
            }
        }
        else
        {
            ++it;
        }
    }
}

void GameState::processEnemyShooting(float deltaTime, const sf::Vector2f &playerPosition)
{
    auto &enemies = gameData.getEnemies();
    auto &projectiles = gameData.getProjectiles();

    for (auto &enemy : enemies)
    {
        if (auto shooterEnemy = dynamic_cast<ShooterEnemy *>(enemy.get()))
        {
            if (auto projectile = shooterEnemy->getShootData(deltaTime, playerPosition))
            {
                projectiles.emplace_back(ProjectileType::BULLET_ALIEN,
                                         gameData.textureManager.getTexture(TextureId::PROJECTILES),
                                         gameData.projectileTextureManager.getTextureRect(ProjectileType::BULLET_ALIEN),
                                         projectile->position,
                                         projectile->direction,
                                         projectile->damage);
            }
        }
    }
}

void GameState::renderScoreText(sf::RenderWindow &window, const sf::View &view)
{
    float margin = 10.0f;
    scoreText.setPosition(
        view.getCenter().x + view.getSize().x / 2.0f - scoreText.getGlobalBounds().width - margin,
        view.getCenter().y - view.getSize().y / 2.0f + margin);

    window.draw(scoreText);
}

void GameState::updateScoreText(int score)
{
    scoreText.setString(GameUtils::formatScoreText(score));
}

void GameState::ensureBackgroundSizeIsLinkedToViewSize(
    const sf::Vector2f &viewPos,
    const sf::Vector2f &viewSize)
{
    background.setSize(viewSize);
    background.setPosition(viewPos);
    background.setTextureRect(sf::IntRect(
        static_cast<int>(viewPos.x),
        static_cast<int>(viewPos.y),
        static_cast<int>(viewSize.x),
        static_cast<int>(viewSize.y)));
}
