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
    int currentWaveIndex;
    float waveCooldown;
    bool waveActive;

    std::unordered_map<EnemyType, int> initialWaveIndex;
    std::vector<Wave> waves;
    Wave activeWave;

    std::unordered_map<EnemyType, EnemyStats> enemyStats;
    std::unordered_map<EnemyType, EnemyStats> activeEnemyStats;

private:
    float getWaveCooldown();
    void startNextWave();
    void applyLevelEnhancementToWave(int levelIndex, int baseWaveIndex);
    void applyLevelBuffsToEnemyStats(int levelIndex, int baseWaveIndex);
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
    const std::unordered_map<EnemyType, EnemyStats> &getEnemyStats() const;
};

#endif // GAMEFLOWMANAGER_H
