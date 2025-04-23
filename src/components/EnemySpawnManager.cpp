#include "constants/Constants.h"
#include "EnemySpawnManager.h"

#include "entities/BasicEnemy.h"
#include "entities/ShooterEnemy.h"
#include "utils/GameUtils.h"

EnemySpawnManager::EnemySpawnManager()
{
    spawnData[EnemyType::BASIC] = {0.0f, Constants::ENEMY_SPAWN_RATE};
    spawnData[EnemyType::SHOOTER] = {0.0f, Constants::ENEMY_SPAWN_RATE * Constants::ENEMY_SPAWN_FACTOR_SHOOTER};

    enemySpeedMap[EnemyType::BASIC] = Constants::BASE_PLAYER_SPEED * 0.5f;
    enemySpeedMap[EnemyType::SHOOTER] = Constants::BASE_PLAYER_SPEED * 0.33f;
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
    const sf::View &view)
{
    for (auto &[enemyType, spawnInfo] : spawnData)
    {
        spawnInfo.spawnAccumulator += spawnInfo.spawnRate * deltaTime;
        int enemiesToSpawn = static_cast<int>(spawnInfo.spawnAccumulator);
        spawnInfo.spawnAccumulator -= enemiesToSpawn;

        for (int i = 0; i < enemiesToSpawn; ++i)
        {
            sf::Vector2f spawnPosition = getRandomSpawnPosition(view);
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
    const sf::View &view)
{
    int side = (rand() % 4) + 1; // Choose a random side around the player
    Direction dir = static_cast<Direction>(side);
    return GameUtils::getRandomPositionOnSide(view, dir, Constants::ENEMY_SPAWN_OFFSET);
}
