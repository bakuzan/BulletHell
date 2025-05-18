#include <iostream>
#include <string>

#include "GameFlowManager.h"

GameFlowManager::GameFlowManager()
    : currentWaveIndex(0),
      waveCooldown(0.5f),
      waveActive(false),
      activeWave(0.0f, {})
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
        activeWave.elapsedTime += deltaTime;

        if (activeWave.elapsedTime > activeWave.duration)
        {
            activeWave.elapsedTime = 0.0f; // reset the time!

            waveActive = false;
            waveCooldown = getWaveCooldown();
            currentWaveIndex++;
        }
    }
    else
    {
        waveCooldown -= deltaTime;

        if (waveCooldown <= 0.0f &&
            enemies.empty())
        {
            startNextWave();
        }
        else if (!enemies.empty())
        {
            waveCooldown = getWaveCooldown();
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

int GameFlowManager::getLevelNumber() const
{
    return (currentWaveIndex / waves.size()) + 1;
}

int GameFlowManager::getWaveNumber() const
{
    int waveNumber = (currentWaveIndex % waves.size()) + 1;
    return waveActive
               ? waveNumber
               : waveNumber - 1;
}

std::unordered_map<EnemyType, float> GameFlowManager::getSpawnRates()
{
    return activeWave.spawnRates;
}

// Privates

float GameFlowManager::getWaveCooldown()
{
    return std::max(0.0f, 6.0f - (1.0f * getLevelNumber()));
}

void GameFlowManager::startNextWave()
{
    waveActive = true;

    int baseWaveIndex = (getWaveNumber() - 1) % waves.size();
    activeWave = waves[baseWaveIndex];
    activeWave.elapsedTime = 0.0f;

    // TODO adjust spawn rates based on level!
}

void GameFlowManager::initialise()
{
    currentWaveIndex = 0;
    waveCooldown = 0.5f;
    waveActive = false;
    waves.clear();

    waves.push_back({10.0f, {{EnemyType::BASIC, 5.0f}, {EnemyType::SHOOTER, 0.1f}, {EnemyType::SPEEDY, 0.0f}, {EnemyType::BOMBER, 0.0f}}});
    waves.push_back({12.0f, {{EnemyType::BASIC, 6.0f}, {EnemyType::SHOOTER, 0.2f}, {EnemyType::SPEEDY, 0.1f}, {EnemyType::BOMBER, 0.0f}}});
    waves.push_back({15.0f, {{EnemyType::BASIC, 7.0f}, {EnemyType::SHOOTER, 0.3f}, {EnemyType::SPEEDY, 0.2f}, {EnemyType::BOMBER, 0.0f}}});
    waves.push_back({18.0f, {{EnemyType::BASIC, 8.0f}, {EnemyType::SHOOTER, 0.5f}, {EnemyType::SPEEDY, 0.3f}, {EnemyType::BOMBER, 0.1f}}});
    waves.push_back({22.0f, {{EnemyType::BASIC, 10.0f}, {EnemyType::SHOOTER, 0.7f}, {EnemyType::SPEEDY, 0.5f}, {EnemyType::BOMBER, 0.3f}}});
    waves.push_back({25.0f, {{EnemyType::BASIC, 12.0f}, {EnemyType::SHOOTER, 1.0f}, {EnemyType::SPEEDY, 0.7f}, {EnemyType::BOMBER, 0.5f}}});

    waves.push_back({1.10f,
                     {{EnemyType::BASIC, 0.0f},
                      {EnemyType::SHOOTER, 0.0f},
                      {EnemyType::SPEEDY, 0.0f},
                      {EnemyType::BOMBER, 0.0f},
                      {EnemyType::BOSS, 1.0f}}});
}
