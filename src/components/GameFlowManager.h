#ifndef GAMEFLOWMANAGER_H
#define GAMEFLOWMANAGER_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "constants/EnemyType.h"
#include "data/Wave.h"
#include "entities/Enemy.h"

class GameFlowManager
{
private:
    int currentLevelIndex;
    int currentWaveIndex;
    float waveCooldown;
    bool waveActive;

    std::vector<Wave> waves;

private:
    void startNextWave();
    void initialise();

public:
    GameFlowManager();
    ~GameFlowManager();

    void update(float deltaTime, const std::vector<std::unique_ptr<Enemy>> &enemies);
    void reset();

    bool isWaveActive() const;
    int getLevelNumber() const;
    int getWaveNumber() const;

    const std::unordered_map<EnemyType, float> &getSpawnRates() const;
};

#endif // GAMEFLOWMANAGER_H
