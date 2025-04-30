#pragma once

#include <unordered_map>

#include "constants/EnemyType.h"

struct Wave
{
    float duration;
    float elapsedTime = 0.0f;
    std::unordered_map<EnemyType, float> spawnRates;

    Wave(float waveDuration, std::unordered_map<EnemyType, float> waveSpawnRates)
        : duration(waveDuration), spawnRates(waveSpawnRates)
    {
    }
};