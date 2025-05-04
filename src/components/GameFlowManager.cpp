#include <iostream>
#include <string>

#include "GameFlowManager.h"

GameFlowManager::GameFlowManager()
    : currentLevelIndex(0), currentWaveIndex(0),
      waveCooldown(0.25f),
      waveActive(false)
{
    initialise();
}

GameFlowManager::~GameFlowManager()
{
    // Destructor
}

// Publics

void GameFlowManager::update(float deltaTime, const std::vector<std::unique_ptr<Enemy>> &enemies)
{
    if (waveActive)
    {
        Wave &currentWave = waves[currentWaveIndex];
        currentWave.elapsedTime += deltaTime;

        if (currentWave.elapsedTime > currentWave.duration)
        {
            currentWave.elapsedTime = 0.0f; // reset the time!

            waveActive = false;
            waveCooldown = 10.0f;
            currentWaveIndex++;
        }
    }
    else
    {
        waveCooldown -= deltaTime;

        if (waveCooldown <= 0.0f &&
            (currentWaveIndex < waves.size() ||
             enemies.empty()))
        {

            startNextWave();
        }
        else if (currentWaveIndex == waves.size() &&
                 !enemies.empty())
        {
            waveCooldown = 5.0f;
        }
    }
}

void GameFlowManager::reset()
{
    initialise();
}

bool GameFlowManager::isWaveActive() const
{
    return waveActive;
}

const std::unordered_map<EnemyType, float> &GameFlowManager::getSpawnRates() const
{
    return waves[currentWaveIndex].spawnRates;
}

// Privates

void GameFlowManager::startNextWave()
{
    if (currentWaveIndex >= waves.size())
    {
        currentLevelIndex++;
        currentWaveIndex = 0;
    }

    waveActive = true;
}

void GameFlowManager::initialise()
{
    currentLevelIndex = 0;
    currentWaveIndex = 0;
    waveCooldown = 0.25f;
    waveActive = false;
    waves.clear();

    // waves.push_back({10.0f, {{EnemyType::BASIC, 5.0f}, {EnemyType::SHOOTER, 0.1f}, {EnemyType::SPEEDY, 0.0f}, {EnemyType::BOMBER, 0.0f}}});
    // waves.push_back({12.0f, {{EnemyType::BASIC, 6.0f}, {EnemyType::SHOOTER, 0.2f}, {EnemyType::SPEEDY, 0.1f}, {EnemyType::BOMBER, 0.0f}}});
    // waves.push_back({15.0f, {{EnemyType::BASIC, 7.0f}, {EnemyType::SHOOTER, 0.3f}, {EnemyType::SPEEDY, 0.2f}, {EnemyType::BOMBER, 0.0f}}});
    // waves.push_back({18.0f, {{EnemyType::BASIC, 8.0f}, {EnemyType::SHOOTER, 0.5f}, {EnemyType::SPEEDY, 0.3f}, {EnemyType::BOMBER, 0.1f}}});
    // waves.push_back({22.0f, {{EnemyType::BASIC, 10.0f}, {EnemyType::SHOOTER, 0.7f}, {EnemyType::SPEEDY, 0.5f}, {EnemyType::BOMBER, 0.3f}}});
    // waves.push_back({25.0f, {{EnemyType::BASIC, 12.0f}, {EnemyType::SHOOTER, 1.0f}, {EnemyType::SPEEDY, 0.7f}, {EnemyType::BOMBER, 0.5f}}});

    waves.push_back({1.10f,
                     {{EnemyType::BASIC, 0.0f},
                      {EnemyType::SHOOTER, 0.0f},
                      {EnemyType::SPEEDY, 0.0f},
                      {EnemyType::BOMBER, 0.0f},
                      {EnemyType::BOSS, 1.0f}}});
}
