#ifndef ENEMYSPAWNMANAGER_H
#define ENEMYSPAWNMANAGER_H

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <unordered_map>

#include "components/EnemyTextureManager.h"
#include "entities/Enemy.h"

class EnemySpawnManager
{
private:
    EnemyTextureManager enemyTextureManager;

    struct SpawnInfo
    {
        float spawnAccumulator = 0.0f; // Accumulated time
        float spawnRate;               // Spawns per second
    };

    std::unordered_map<EnemyType, SpawnInfo> spawnData;
    std::unordered_map<EnemyType, float> enemySpeedMap;

private:
    std::unique_ptr<Enemy> spawnEnemy(
        EnemyType type,
        const sf::Texture &texture,
        sf::IntRect textureRect,
        sf::Vector2f spawnPosition,
        float speed);

    sf::Vector2f getRandomSpawnPosition(
        const sf::Vector2f &playerPosition,
        const sf::View &view);

public:
    EnemySpawnManager();
    ~EnemySpawnManager();

    void spawnEnemies(
        float deltaTime,
        std::vector<std::unique_ptr<Enemy>> &enemies,
        const sf::Texture &enemiesTexture,
        const sf::Vector2f &playerPos,
        const sf::View &view);
};

#endif // ENEMYSPAWNMANAGER_H
