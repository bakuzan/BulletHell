#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#include "entities/BasicEnemy.h"
#include "entities/ShooterEnemy.h"
#include "utils/GameUtils.h"
#include "utils/InputUtils.h"
#include "constants/Constants.h"
#include "GameState.h"
#include "PauseState.h"
#include "GameOverState.h"

GameState::GameState(GameData &data, StateManager &manager, sf::RenderWindow &window)
    : gameData(data),
      stateManager(manager),
      window(window),
      healthBar(gameData.textureManager.getTexture(TextureId::HEALTHBAR_BORDER),
                gameData.textureManager.getTexture(TextureId::HEALTHBAR_FILLINGS),
                300.0f,
                100.0f)
{
    // Load background
    const sf::Texture &backgroundTexture = gameData.textureManager.getTexture(TextureId::BACKGROUND);
    background.setTexture(&backgroundTexture);

    // Setup points display
    scoreText.setFont(gameData.gameFont);
    scoreText.setString(std::string(8, '0'));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(10, 50);

    // Set up the view + adjust background
    view.setSize(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT);
    view.setCenter(gameData.getPlayer()->getSprite().getPosition());
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

    gameData.getPlayer()->handleEvent(event);
}

void GameState::update(sf::Time deltaTime, sf::RenderWindow &window)
{
    enemySpawnManager.spawnEnemies(
        deltaTime.asSeconds(),
        gameData.getEnemies(),
        gameData.textureManager.getTexture(TextureId::SPACESHIPS),
        view);

    auto &player = gameData.getPlayer();
    auto playerSprite = player->getSprite();
    updateEnemies(deltaTime.asSeconds(), playerSprite.getPosition());
    processEnemyShooting(deltaTime.asSeconds(), playerSprite.getPosition());

    player->update(deltaTime.asSeconds(), window);
    if (auto playerProjectile = player->getShootData())
    {
        projectileSpawnManager.spawnPlayerProjectile(
            gameData.textureManager.getTexture(TextureId::PROJECTILES),
            gameData.getProjectiles(),
            *playerProjectile);
    }

    updateProjectiles(deltaTime, window);

    upgradeBoxSpawnManager.spawnUpgradeBoxes(
        deltaTime.asSeconds(),
        gameData.getUpgradeBoxes(),
        gameData.textureManager.getTexture(TextureId::UPGRADE_BOXES),
        player->getLastDirectionMoved(),
        view);
    updateUpgradeBoxes(deltaTime);

    // Update view to follow player
    view.setCenter(playerSprite.getPosition());

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

    auto &upgrades = gameData.getUpgradeBoxes();
    for (const auto &upgrade : upgrades)
    {
        upgrade.render(window);
    }

    gameData.getPlayer()->render(window);

    auto &enemies = gameData.getEnemies();
    for (const auto &enemy : enemies)
    {
        enemy->render(window);
    }

    auto &projectiles = gameData.getProjectiles();
    for (const auto &projectile : projectiles)
    {
        projectile.render(window);
    }

    // UI Elements
    healthBar.render(window, view);
    renderScoreText(window, view);
}

// Private

void GameState::updateProjectiles(const sf::Time &deltaTime, sf::RenderWindow &window)
{
    auto &projectiles = gameData.getProjectiles();
    auto &enemies = gameData.getEnemies();
    auto &player = gameData.getPlayer();

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
                    (*enemyIt)->updateHealth(-projIt->getDamageInflicts());
                    if ((*enemyIt)->getHealth() <= 0.0f)
                    {
                        gameData.updateScore((*enemyIt)->getPointsValue());
                        updateScoreText(gameData.getScore());

                        enemyIt = enemies.erase(enemyIt);
                    }

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
                 projIt->getSprite().getGlobalBounds().intersects(player->getSprite().getGlobalBounds()))
        {
            projIt = projectiles.erase(projIt);
            projectileRemoved = true;

            // Effects of player being hit
            player->updateHealth(-projIt->getDamageInflicts());
            healthBar.setHealth(player->getHealth());

            if (player->getHealth() <= 0)
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

void GameState::updateUpgradeBoxes(const sf::Time &deltaTime)
{
    auto &boxes = gameData.getUpgradeBoxes();
    auto &player = gameData.getPlayer();
    sf::FloatRect playerBounds = player->getSprite().getGlobalBounds();

    for (auto boxIt = boxes.begin(); boxIt != boxes.end();)
    {
        boxIt->update(deltaTime);

        if (boxIt->isExpired())
        {
            boxIt = boxes.erase(boxIt);
        }
        else if (boxIt->getSprite().getGlobalBounds().intersects(playerBounds))
        {
            processUpgradeBoxPickUp(*boxIt);
            boxIt = boxes.erase(boxIt);
        }
        else
        {
            ++boxIt;
        }
    }
}

void GameState::processUpgradeBoxPickUp(const UpgradeBox &upgradeBox)
{
    switch (upgradeBox.getType())
    {
    case UpgradeBoxType::HEALTH:
    {
        auto &player = gameData.getPlayer();
        player->updateHealth(50);
        healthBar.setHealth(player->getHealth());
        break;
    }
    default:
        std::cerr << "Unknown upgrade box type!" << std::endl;
        break;
    }
}

void GameState::updateEnemies(float deltaTime, const sf::Vector2f &playerPosition)
{
    auto &enemies = gameData.getEnemies();
    auto &player = gameData.getPlayer();
    sf::FloatRect playerBounds = player->getSprite().getGlobalBounds();

    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();)
    {
        (*enemyIt)->update(deltaTime, playerPosition);

        if ((*enemyIt)->getSprite().getGlobalBounds().intersects(playerBounds))
        {
            // Effects of hitting player
            float collisionDamage = (*enemyIt)->getHealth() * 0.4f;
            player->updateHealth(-collisionDamage);
            healthBar.setHealth(player->getHealth());

            enemyIt = enemies.erase(enemyIt); // Current assumption will be enemies die on collision!

            if (player->getHealth() <= 0)
            {
                stateManager.pushState(std::make_unique<GameOverState>(gameData, stateManager, window));
            }
        }
        else
        {
            ++enemyIt;
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
                projectileSpawnManager.spawnEnemyProjectile(
                    gameData.textureManager.getTexture(TextureId::PROJECTILES),
                    projectiles,
                    *projectile);
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
