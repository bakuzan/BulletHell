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

    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>> &enemies);
    void reset();
    // TODO Consider a mechanism to display ui for wave num/cooldown
    bool isWaveActive() const;

    const std::unordered_map<EnemyType, float> &getSpawnRates() const;
};

#endif // GAMEFLOWMANAGER_H
