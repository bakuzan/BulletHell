#include "constants/Constants.h"
#include "EnemySpawnManager.h"

#include "entities/BasicEnemy.h"
#include "entities/ShooterEnemy.h"
#include "entities/SpeedyEnemy.h"
#include "utils/GameUtils.h"

EnemySpawnManager::EnemySpawnManager()
{
    spawnData[EnemyType::BASIC] = {0.0f, 0.0f};
    spawnData[EnemyType::SHOOTER] = {0.0f, 0.0f};
    spawnData[EnemyType::SPEEDY] = {0.0f, 0.0f};

    enemySpeedMap[EnemyType::BASIC] = Constants::BASE_PLAYER_SPEED * 0.50f;
    enemySpeedMap[EnemyType::SHOOTER] = Constants::BASE_PLAYER_SPEED * 0.33f;
    enemySpeedMap[EnemyType::SPEEDY] = Constants::BASE_PLAYER_SPEED * 1.10f;
}

EnemySpawnManager::~EnemySpawnManager()
{
    // Destructor
}

// Publics

void EnemySpawnManager::setWaveParameters(
    const std::unordered_map<EnemyType, float> &spawnRates)
{
    for (auto &[enemyType, spawnInfo] : spawnData)
    {
        if (spawnRates.count(enemyType))
        {
            spawnInfo.spawnRate = spawnRates.at(enemyType);
        }
    }
}

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
    case EnemyType::SPEEDY:
        return std::make_unique<SpeedyEnemy>(texture, textureRect, spawnPosition, speed);
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
