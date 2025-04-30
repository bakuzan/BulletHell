#include <iostream>
#include <string>

#include "GameFlowManager.h"

GameFlowManager::GameFlowManager()
    : currentWaveIndex(0),
      waveCooldown(0.25f),
      waveActive(false)
{
    waves.push_back({15.0f,
                     {{EnemyType::BASIC, 5.0f},
                      {EnemyType::SHOOTER, 0.2f},
                      {EnemyType::SPEEDY, 0.0f}}});
    waves.push_back({15.0f,
                     {{EnemyType::BASIC, 6.0f},
                      {EnemyType::SHOOTER, 0.3f},
                      {EnemyType::SPEEDY, 0.1f}}});
    waves.push_back({20.0f,
                     {{EnemyType::BASIC, 7.0f},
                      {EnemyType::SHOOTER, 0.4f},
                      {EnemyType::SPEEDY, 0.2f}}});
    waves.push_back({20.0f,
                     {{EnemyType::BASIC, 8.0f},
                      {EnemyType::SHOOTER, 0.4f},
                      {EnemyType::SPEEDY, 0.3f}}});
    waves.push_back({25.0f,
                     {{EnemyType::BASIC, 9.0f},
                      {EnemyType::SHOOTER, 0.5f},
                      {EnemyType::SPEEDY, 0.4f}}});
}

GameFlowManager::~GameFlowManager()
{
    // Destructor
}

// Publics

void GameFlowManager::update(float deltaTime)
{
    if (waveActive)
    {
        Wave &currentWave = waves[currentWaveIndex];
        currentWave.elapsedTime += deltaTime;

        if (currentWave.elapsedTime > currentWave.duration)
        {
            waveActive = false;
            waveCooldown = 5.0f;
            currentWaveIndex++;
        }
    }
    else
    {
        waveCooldown -= deltaTime;

        if (waveCooldown <= 0.0f)
        {
            startNextWave();
        }
    }
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
    if (currentWaveIndex < waves.size())
    {
        waveActive = true;
    }
    else
    {
        // TODO reset waveIndex=0 and move up a level.
        std::cerr << "Wave limit reached at currentWaveIndex="
                  << std::to_string(currentWaveIndex)
                  << std::endl;
    }
}
