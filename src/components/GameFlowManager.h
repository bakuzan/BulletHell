#ifndef GAMEFLOWMANAGER_H
#define GAMEFLOWMANAGER_H

#include <unordered_map>
#include <vector>

#include "constants/EnemyType.h"
#include "data/Wave.h"

class GameFlowManager
{
private:
    int currentWaveIndex;
    float waveCooldown;
    bool waveActive;

    std::vector<Wave> waves;

private:
    void startNextWave();

public:
    GameFlowManager();
    ~GameFlowManager();

    void update(float deltaTime);
    bool isWaveActive() const;

    const std::unordered_map<EnemyType, float> &getSpawnRates() const;
};

#endif // GAMEFLOWMANAGER_H
