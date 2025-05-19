#include "constants/Constants.h"
#include "EnemySpawnManager.h"

#include "entities/BasicEnemy.h"
#include "entities/ShooterEnemy.h"
#include "entities/SpeedyEnemy.h"
#include "entities/BomberEnemy.h"
#include "entities/BossEnemy.h"
#include "utils/GameUtils.h"

EnemySpawnManager::EnemySpawnManager(const TextureRectManager &textureRectManager)
    : enemyRectManager(textureRectManager)
{
    spawnData[EnemyType::BASIC] = {0.0f, 0.0f};
    spawnData[EnemyType::SHOOTER] = {0.0f, 0.0f};
    spawnData[EnemyType::SPEEDY] = {0.0f, 0.0f};
    spawnData[EnemyType::BOMBER] = {0.0f, 0.0f};
    spawnData[EnemyType::BOSS] = {0.0f, 0.0f};

    statsData[EnemyType::BASIC] = EnemyStats::CreateEmpty();
    statsData[EnemyType::SHOOTER] = EnemyStats::CreateEmpty();
    statsData[EnemyType::SPEEDY] = EnemyStats::CreateEmpty();
    statsData[EnemyType::BOMBER] = EnemyStats::CreateEmpty();
    statsData[EnemyType::BOSS] = EnemyStats::CreateEmpty();
}

EnemySpawnManager::~EnemySpawnManager()
{
    // Destructor
}

// Publics

void EnemySpawnManager::setWaveParameters(
    const std::unordered_map<EnemyType, float> &spawnRates,
    const std::unordered_map<EnemyType, EnemyStats> &stats)
{
    for (auto &[enemyType, spawnInfo] : spawnData)
    {
        spawnInfo.spawnRate = spawnRates.count(enemyType)
                                  ? spawnRates.at(enemyType)
                                  : 0.0f;
    }

    for (auto &[enemyType, statsInfo] : statsData)
    {
        statsInfo = stats.count(enemyType)
                        ? stats.at(enemyType)
                        : EnemyStats::CreateEmpty();
    }
}

void EnemySpawnManager::spawnEnemies(
    const TextureManager &textureManager,
    float deltaTime,
    std::vector<std::unique_ptr<Enemy>> &enemies,
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
            const auto &textureRect = enemyRectManager.getTextureRect(enemyType);
            EnemyStats enemyStats = statsData[enemyType];

            enemies.emplace_back(
                spawnEnemy(enemyType,
                           textureManager, textureRect,
                           spawnPosition, enemyStats));
        }
    }
}

// Privates

std::unique_ptr<Enemy> EnemySpawnManager::spawnEnemy(
    EnemyType type,
    const TextureManager &textureManager,
    sf::IntRect textureRect,
    sf::Vector2f spawnPosition,
    EnemyStats enemyStats)
{
    const sf::Texture &texture = textureManager.getTexture(TextureId::SPACESHIPS);

    switch (type)
    {
    case EnemyType::BASIC:
        return std::make_unique<BasicEnemy>(texture, textureRect, spawnPosition, enemyStats);
    case EnemyType::SHOOTER:
        return std::make_unique<ShooterEnemy>(texture, textureRect, spawnPosition, enemyStats);
    case EnemyType::SPEEDY:
        return std::make_unique<SpeedyEnemy>(texture, textureRect, spawnPosition, enemyStats);
    case EnemyType::BOMBER:
        return std::make_unique<BomberEnemy>(texture, textureRect, spawnPosition, enemyStats);
    case EnemyType::BOSS:
        return std::make_unique<BossEnemy>(
            textureManager.getTexture(TextureId::HEALTHBAR_BORDER),
            textureManager.getTexture(TextureId::HEALTHBAR_FILLINGS),
            texture, textureRect, spawnPosition, enemyStats);
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
