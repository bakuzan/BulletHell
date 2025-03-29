#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <SFML/System/Vector2.hpp>

#include <memory>

#include "Enemy.h"
#include "EnemyTextureManager.h"
#include "Projectile.h"
#include "ProjectileTextureManager.h"
#include "TextureManager.h"

class GameData
{
private:
    std::vector<Projectile> projectiles;
    std::vector<std::unique_ptr<Enemy>> enemies;

    float playerHealth;
    int playerScore;

public:
    TextureManager textureManager;
    ProjectileTextureManager projectileTextureManager;
    EnemyTextureManager enemyTextureManager;

public:
    GameData();
    ~GameData();

    std::vector<Projectile> &getProjectiles();
    void addProjectile(const Projectile &projectile);
    void removeProjectile(size_t index);

    std::vector<std::unique_ptr<Enemy>> &getEnemies();
    void addEnemy(std::unique_ptr<Enemy> enemy);
    void removeEnemy(size_t index);

    const float getPlayerHealth() const;
    void updatePlayerHealth(float adjustment);
    const int getScore() const;
    void updateScore(int adjustment);

    void reset();
};

#endif // GAMEDATA_H
