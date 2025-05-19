#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include "data/PairHash.h"
#include "entities/ShooterEnemy.h"
#include "entities/BomberEnemy.h"
#include "entities/BossEnemy.h"
#include "entities/LaserProjectile.h"
#include "entities/SeekerProjectile.h"
#include "utils/CollisionUtils.h"
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
      enemySpawnManager(gameData.rectManager),
      projectileSpawnManager(gameData.rectManager),
      upgradeBoxSpawnManager(gameData.rectManager),
      weaponUIManager(gameData.textureManager.getTexture(TextureId::UPGRADE_BOXES),
                      gameData.rectManager.getUpgradeBoxTextureRects())
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
    if (gameData.audioManager.getSoundStatus(AudioId::AMBIENT) != sf::Sound::Status::Playing)
    {
        gameData.audioManager.playSound(AudioId::AMBIENT, true);
    }

    gameData.gameFlowManager.update(
        deltaTime.asSeconds(),
        gameData.getEnemies());

    auto &player = gameData.getPlayer();
    auto playerSprite = player->getSprite();

    if (gameData.gameFlowManager.isWaveActive())
    {
        enemySpawnManager.setWaveParameters(
            gameData.gameFlowManager.getSpawnRates(),
            gameData.gameFlowManager.getEnemyStats());

        enemySpawnManager.spawnEnemies(
            gameData.textureManager,
            deltaTime.asSeconds(),
            gameData.getEnemies(),
            view);
    }

    updateEnemies(deltaTime.asSeconds(), playerSprite.getPosition());
    processEnemyProjectiles(deltaTime.asSeconds(), playerSprite.getPosition());

    player->update(deltaTime.asSeconds(), window);
    if (auto playerProjectile = player->getShootData())
    {
        projectileSpawnManager.spawnPlayerProjectile(
            gameData.textureManager.getTexture(TextureId::PROJECTILES),
            gameData.getProjectiles(),
            *playerProjectile,
            playerSprite.getRotation());

        AudioId projectileAudioId = GameUtils::getAudioIdForProjectileType(playerProjectile->type);
        gameData.audioManager.playSound(projectileAudioId);
    }

    updateProjectiles(deltaTime, window);

    upgradeBoxSpawnManager.spawnUpgradeBoxes(
        deltaTime.asSeconds(),
        gameData.getUpgradeBoxes(),
        gameData.textureManager.getTexture(TextureId::UPGRADE_BOXES),
        player->getLastDirectionMoved(),
        view);

    updateUpgradeBoxes(deltaTime);

    weaponUIManager.update(player->getWeaponType(),
                           player->getWeaponTimeout());

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

    auto &enemies = gameData.getEnemies();
    for (const auto &enemy : enemies)
    {
        enemy->render(window);
    }

    auto &projectiles = gameData.getProjectiles();
    for (const auto &projectile : projectiles)
    {
        projectile->render(window);
    }

    gameData.getPlayer()->render(window);

    // UI Elements
    sf::View uiView = window.getDefaultView();
    window.setView(uiView); // Set "UI" view

    gameData.getPlayer()->getHealthBar().render(window);
    weaponUIManager.render(window);

    for (const auto &enemy : enemies)
    {
        if (auto boss = dynamic_cast<BossEnemy *>(enemy.get()))
        {
            boss->getHealthBar().render(window);
        }
    }

    renderScoreText(window);

    // Restore game view
    window.setView(view);
}

// Private

void GameState::updateProjectiles(const sf::Time &deltaTime, sf::RenderWindow &window)
{
    std::vector<ProjectileData> newProjectilesData;
    auto &projectiles = gameData.getProjectiles();
    auto &enemies = gameData.getEnemies();
    auto &player = gameData.getPlayer();
    sf::Vector2f playerPos = player->getSprite().getPosition();

    // Setup spatial grid for projectile-projectile collision detection
    std::unordered_map<std::pair<int, int>, std::vector<Projectile *>, PairHash> spatialGrid;
    int gridCellSize = 64; // TODO (Optional) Make dynamic if performance is an issue

    for (auto projIt = projectiles.begin(); projIt != projectiles.end();)
    {
        auto projectile = projIt->get();

        // Setup
        if (projectile->getType() == ProjectileType::ALIEN_SEEKER)
        {
            auto seeker = dynamic_cast<SeekerProjectile *>(projectile);
            seeker->setTargetPosition(playerPos);
        }

        // Update
        projectile->update(deltaTime);

        // Handle collisions
        bool projectileRemoved = false;
        if (projectile->getType() == ProjectileType::LASER ||
            projectile->getType() == ProjectileType::ALIEN_LASER)
        {
            auto laser = dynamic_cast<LaserProjectile *>(projectile);

            if (!laser->isDamageCalculated())
            {
                if (laser->getOrigin() == ProjectileOrigin::PLAYER)
                {
                    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();)
                    {
                        auto &enemy = **enemyIt;
                        if (laser->getSprite().getGlobalBounds().intersects(enemy.getSprite().getGlobalBounds()) &&
                            CollisionUtils::CheckSpritesIntersect(laser->getSprite(), enemy.getSprite()))
                        {
                            enemy.updateHealth(-laser->getDamageInflicts());
                            if (enemy.getHealth() <= 0.0f)
                            {
                                gameData.updateScore(enemy.getPointsValue());
                                updateScoreText(gameData.getScore());

                                enemyIt = enemies.erase(enemyIt);
                            }
                            else
                            {
                                ++enemyIt;
                            }
                        }
                        else
                        {
                            ++enemyIt;
                        }
                    }
                }
                else if (laser->getSprite().getGlobalBounds().intersects(player->getSprite().getGlobalBounds()) &&
                         CollisionUtils::CheckSpritesIntersect(laser->getSprite(), player->getSprite()))
                {
                    // Effects of player being hit
                    player->updateHealth(-laser->getDamageInflicts());

                    if (player->getHealth() <= 0)
                    {
                        onPlayerDeath();
                    }
                }

                laser->setDamageCalculated(true);
                auto laserCells = CollisionUtils::getLaserCells(gridCellSize,
                                                                laser->getStartPoint(),
                                                                laser->getEndPoint());

                for (auto &cell : laserCells)
                {
                    spatialGrid[cell].push_back(projectile);
                }
            }

            if (laser->canBeRemoved())
            {
                projIt = projectiles.erase(projIt);
            }
            else
            {
                ++projIt;
            }

            continue;
        }

        if (projectile->getOrigin() == ProjectileOrigin::PLAYER)
        {
            for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();)
            {
                auto &enemy = **enemyIt;
                if (projectile->getSprite().getGlobalBounds().intersects(enemy.getSprite().getGlobalBounds()) &&
                    CollisionUtils::CheckSpritesIntersect(projectile->getSprite(), enemy.getSprite()))
                {
                    // Effects of shooting enemy
                    enemy.updateHealth(-projectile->getDamageInflicts());
                    if (enemy.getHealth() <= 0.0f)
                    {
                        gameData.updateScore(enemy.getPointsValue());
                        updateScoreText(gameData.getScore());

                        enemyIt = enemies.erase(enemyIt);
                    }

                    if (projectile->getType() == ProjectileType::MISSILE)
                    {
                        newProjectilesData.push_back(ProjectileData::CreateChained(
                            ProjectileType::MISSILE_DEBRIS,
                            projectile->getSprite().getPosition(),
                            Constants::PROJECTILE_DAMAGE_MISSILE_DEBRIS,
                            Constants::PROJECTILE_SPEED_MISSILE_DEBRIS,
                            Constants::PROJECTILE_MISSILE_DEBRIS_COUNT));
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
        else if (projectile->getOrigin() == ProjectileOrigin::ENEMY &&
                 projectile->getSprite().getGlobalBounds().intersects(player->getSprite().getGlobalBounds()) &&
                 CollisionUtils::CheckSpritesIntersect(projectile->getSprite(), player->getSprite()))
        {
            projIt = projectiles.erase(projIt);
            projectileRemoved = true;

            // Effects of player being hit
            player->updateHealth(-projectile->getDamageInflicts());

            if (player->getHealth() <= 0)
            {
                onPlayerDeath();
            }
        }

        if (!projectileRemoved)
        {
            if (projectile->isOutOfPlay(window))
            {
                projIt = projectiles.erase(projIt);
            }
            else
            {
                // Assign to cell grid
                auto cell = CollisionUtils::getGridCell(gridCellSize, projectile->getSprite().getPosition());
                spatialGrid[cell].push_back(projectile);

                ++projIt;
            }
        }
    }

    // Handle projectile-projectile collisions!
    std::unordered_set<Projectile *> removalList;
    for (auto &[cell, projectilesInCell] : spatialGrid)
    {
        for (size_t i = 0; i < projectilesInCell.size(); ++i)
        {
            for (size_t j = i + 1; j < projectilesInCell.size(); ++j)
            {
                auto *projA = projectilesInCell[i];
                auto *projB = projectilesInCell[j];

                if (projA->getSprite().getGlobalBounds().intersects(projB->getSprite().getGlobalBounds()) &&
                    CollisionUtils::CheckSpritesIntersect(projA->getSprite(), projB->getSprite()))
                {
                    if (projA->getOrigin() != projB->getOrigin())
                    {
                        removalList.insert(projA);
                        removalList.insert(projB);
                    }
                }
            }
        }
    }

    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
                                     [&](const std::unique_ptr<Projectile> &projectile)
                                     {
                                         auto proj = projectile.get();
                                         bool isRemove = removalList.count(proj) &&
                                                         proj->getType() != ProjectileType::LASER &&
                                                         proj->getType() != ProjectileType::ALIEN_LASER;

                                         if (isRemove &&
                                             proj->getType() == ProjectileType::MISSILE)
                                         {
                                             newProjectilesData.push_back(ProjectileData::CreateChained(
                                                 ProjectileType::MISSILE_DEBRIS,
                                                 proj->getSprite().getPosition(),
                                                 Constants::PROJECTILE_DAMAGE_MISSILE_DEBRIS,
                                                 Constants::PROJECTILE_SPEED_MISSILE_DEBRIS,
                                                 Constants::PROJECTILE_MISSILE_DEBRIS_COUNT));
                                         }

                                         return isRemove;
                                     }),
                      projectiles.end());

    processChainedProjectiles(projectiles, newProjectilesData);
}

void GameState::processChainedProjectiles(
    std::vector<std::unique_ptr<Projectile>> &projectiles,
    std::vector<ProjectileData> &newProjectilesData)
{
    projectileSpawnManager.spawnChainedProjectiles(
        gameData.textureManager.getTexture(TextureId::PROJECTILES),
        projectiles,
        newProjectilesData);

    for (const auto &proj : newProjectilesData)
    {
        AudioId projectileAudioId = GameUtils::getAudioIdForProjectileType(proj.type);
        gameData.audioManager.playPooledSound(projectileAudioId);
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
        gameData.audioManager.playSound(AudioId::UPGRADEBOX_HEALTH);
        break;
    }
    case UpgradeBoxType::DOUBLE_SHOT:
    case UpgradeBoxType::LASER:
    case UpgradeBoxType::MISSILE:
    {
        auto &player = gameData.getPlayer();
        player->setWeaponType(GameUtils::mapUpgradeBoxToWeapon(upgradeBox.getType()));
        gameData.audioManager.playSound(AudioId::UPGRADEBOX_WEAPON);
        break;
    }
    case UpgradeBoxType::COUNT:
    default:
        std::cerr << "Unknown upgrade box type! = "
                  << std::to_string(static_cast<int>(upgradeBox.getType()))
                  << std::endl;
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
        auto enemy = enemyIt->get();
        enemy->update(deltaTime, window, playerPosition);

        if (enemy->getSprite().getGlobalBounds().intersects(playerBounds) &&
            CollisionUtils::CheckSpritesIntersect(enemy->getSprite(), player->getSprite()))
        {
            // Effects of hitting player
            float collisionDamage = enemy->getHealth() * 0.4f;
            player->updateHealth(-collisionDamage);

            enemyIt = enemies.erase(enemyIt); // Current assumption will be enemies die on collision!

            if (player->getHealth() <= 0)
            {
                onPlayerDeath();
            }
        }
        else
        {
            ++enemyIt;
        }
    }
}

void GameState::processEnemyProjectiles(float deltaTime, const sf::Vector2f &playerPosition)
{
    std::vector<ProjectileData> newProjectilesData;
    auto &enemies = gameData.getEnemies();
    auto &projectiles = gameData.getProjectiles();

    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();)
    {
        auto enemy = enemyIt->get();
        if (auto rangedEnemy = dynamic_cast<RangedEnemy *>(enemy))
        {
            if (auto projectile = rangedEnemy->getShootData(deltaTime, playerPosition))
            {
                projectileSpawnManager.spawnEnemyProjectile(
                    gameData.textureManager.getTexture(TextureId::PROJECTILES),
                    projectiles,
                    *projectile);

                AudioId projectileAudioId = GameUtils::getAudioIdForProjectileType(projectile->type);
                gameData.audioManager.playPooledSound(projectileAudioId);
            }
        }
        else if (auto bomberEnemy = dynamic_cast<BomberEnemy *>(enemy))
        {
            if (bomberEnemy->shouldDetonate(playerPosition))
            {
                newProjectilesData.push_back(ProjectileData::CreateChained(
                    ProjectileType::ALIEN_DEBRIS,
                    bomberEnemy->getSprite().getPosition(),
                    Constants::PROJECTILE_DAMAGE_ALIEN_DEBRIS,
                    Constants::PROJECTILE_SPEED_ALIEN_DEBRIS,
                    24));

                enemyIt = enemies.erase(enemyIt);
                continue;
            }
        }

        ++enemyIt;
    }

    processChainedProjectiles(projectiles, newProjectilesData);
}

void GameState::onPlayerDeath()
{
    gameData.audioManager.playSound(AudioId::DEATH_PLAYER);
    stateManager.pushState(std::make_unique<GameOverState>(gameData, stateManager, window));
}

void GameState::renderScoreText(sf::RenderWindow &window)
{
    sf::View view = window.getDefaultView();

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
