#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <SFML/System/Vector2.hpp>

#include <memory>

#include "Enemy.h"
#include "EnemyTextureManager.h"
#include "Projectile.h"
#include "ProjectileTextureManager.h"

class GameData
{
private:
    std::vector<Projectile> projectiles;
    std::vector<std::unique_ptr<Enemy>> enemies;

public:
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

    void reset();
};

#endif // GAMEDATA_H
