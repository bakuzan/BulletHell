#ifndef PROJECTILESPAWNMANAGER_H
#define PROJECTILESPAWNMANAGER_H

#include "ProjectileTextureManager.h"

#include "constants/ProjectileType.h"
#include "data/ProjectileData.h"
#include "entities/Projectile.h"

class ProjectileSpawnManager
{
private:
    ProjectileTextureManager projectileTextureManager;

public:
    ProjectileSpawnManager();
    ~ProjectileSpawnManager();

    void spawnPlayerProjectile(const sf::Texture &texture,
                               std::vector<Projectile> &projectiles,
                               ProjectileData &data);

    void spawnEnemyProjectile(const sf::Texture &texture,
                              std::vector<Projectile> &projectiles,
                              ProjectileData &data);
};

#endif // PROJECTILESPAWNMANAGER_H
