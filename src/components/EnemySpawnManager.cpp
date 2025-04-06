#include "Constants.h"
#include "EnemySpawnManager.h"

#include "entities/BasicEnemy.h"
#include "entities/ShooterEnemy.h"

EnemySpawnManager::EnemySpawnManager()
{
    spawnData[EnemyType::BASIC] = {0.0f, Constants::ENEMY_SPAWN_RATE};
    spawnData[EnemyType::SHOOTER] = {0.0f, Constants::ENEMY_SPAWN_RATE / Constants::ENEMY_SPAWN_FACTOR_SHOOTER};

    enemySpeedMap[EnemyType::BASIC] = Constants::BASE_PLAYER_SPEED * 0.33f;
    enemySpeedMap[EnemyType::SHOOTER] = Constants::BASE_PLAYER_SPEED * 0.2f;
}

EnemySpawnManager::~EnemySpawnManager()
{
    // Destructor
}

// Publics

void EnemySpawnManager::spawnEnemies(
    float deltaTime,
    std::vector<std::unique_ptr<Enemy>> &enemies,
    const sf::Texture &enemiesTexture,
    const sf::Vector2f &playerPos,
    const sf::View &view)
{
    for (auto &[enemyType, spawnInfo] : spawnData)
    {
        spawnInfo.spawnAccumulator += spawnInfo.spawnRate * deltaTime;
        int enemiesToSpawn = static_cast<int>(spawnInfo.spawnAccumulator);
        spawnInfo.spawnAccumulator -= enemiesToSpawn;

        for (int i = 0; i < enemiesToSpawn; ++i)
        {
            sf::Vector2f spawnPosition = getRandomSpawnPosition(playerPos, view);
            const auto &textureRect = enemyTextureManager.getTextureRect(enemyType);
            float enemySpeed = enemySpeedMap[enemyType];

            enemies.emplace_back(
                spawnEnemy(enemyType,
                           enemiesTexture, textureRect,
                           spawnPosition, enemySpeed));
        }
    }
}

// Privates

std::unique_ptr<Enemy> EnemySpawnManager::spawnEnemy(
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
    case EnemyType::SHOOTER:
        return std::make_unique<ShooterEnemy>(texture, textureRect, spawnPosition, speed);
    default:
        return nullptr;
    }
}

sf::Vector2f EnemySpawnManager::getRandomSpawnPosition(
    const sf::Vector2f &playerPosition,
    const sf::View &view)
{
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f viewCenter = view.getCenter();
    int side = rand() % 4; // Choose a random side around the player

    switch (side)
    {
    case 0: // Top
        return {viewCenter.x - viewSize.x / 2.f + static_cast<float>(rand() % int(viewSize.x)),
                viewCenter.y - viewSize.y / 2.f - Constants::ENEMY_SPAWN_OFFSET};
    case 1: // Left
        return {viewCenter.x - viewSize.x / 2.f - Constants::ENEMY_SPAWN_OFFSET,
                viewCenter.y - viewSize.y / 2.f + static_cast<float>(rand() % int(viewSize.y))};
    case 2: // Right
        return {viewCenter.x + viewSize.x / 2.f + Constants::ENEMY_SPAWN_OFFSET,
                viewCenter.y - viewSize.y / 2.f + static_cast<float>(rand() % int(viewSize.y))};
    case 3: // Bottom
        return {viewCenter.x - viewSize.x / 2.f + static_cast<float>(rand() % int(viewSize.x)),
                viewCenter.y + viewSize.y / 2.f + Constants::ENEMY_SPAWN_OFFSET};
    default:
        return {0.f, 0.f}; // Fallback (this should never occur)
    }
}