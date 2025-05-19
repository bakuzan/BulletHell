#ifndef ENEMYSPAWNMANAGER_H
#define ENEMYSPAWNMANAGER_H

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <unordered_map>

#include "core/TextureManager.h"
#include "components/TextureRectManager.h"
#include "entities/Enemy.h"
#include "data/EnemyStats.h"

class EnemySpawnManager
{
private:
    const TextureRectManager &enemyRectManager;

    struct SpawnInfo
    {
        float spawnAccumulator = 0.0f; // Accumulated time
        float spawnRate;               // Spawns per second
    };

    std::unordered_map<EnemyType, SpawnInfo> spawnData;
    std::unordered_map<EnemyType, EnemyStats> statsData;

private:
    std::unique_ptr<Enemy> spawnEnemy(
        EnemyType type,
        const TextureManager &textureManager,
        sf::IntRect textureRect,
        sf::Vector2f spawnPosition,
        EnemyStats enemyStats);

    sf::Vector2f getRandomSpawnPosition(
        const sf::View &view);

public:
    EnemySpawnManager(const TextureRectManager &textureRectManager);
    ~EnemySpawnManager();

    void setWaveParameters(
        const std::unordered_map<EnemyType, float> &spawnRates,
        const std::unordered_map<EnemyType, EnemyStats> &stats);

    void spawnEnemies(
        const TextureManager &textureManager,
        float deltaTime,
        std::vector<std::unique_ptr<Enemy>> &enemies,
        const sf::View &view);
};

#endif // ENEMYSPAWNMANAGER_H
