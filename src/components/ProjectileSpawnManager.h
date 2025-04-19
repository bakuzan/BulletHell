#ifndef PROJECTILESPAWNMANAGER_H
#define PROJECTILESPAWNMANAGER_H

#include <memory>

#include "ProjectileTextureManager.h"

#include "constants/ProjectileType.h"
#include "data/ProjectileData.h"
#include "entities/Projectile.h"

class ProjectileSpawnManager
{
private:
    ProjectileTextureManager projectileTextureManager;

private:
    std::unique_ptr<Projectile> spawnProjectile(
        const sf::Texture &texture,
        ProjectileData &data);

public:
    ProjectileSpawnManager();
    ~ProjectileSpawnManager();

    void spawnPlayerProjectile(const sf::Texture &texture,
                               std::vector<std::unique_ptr<Projectile>> &projectiles,
                               ProjectileData &data,
                               float playerRotation);

    void spawnEnemyProjectile(const sf::Texture &texture,
                              std::vector<std::unique_ptr<Projectile>> &projectiles,
                              ProjectileData &data);
};

#endif // PROJECTILESPAWNMANAGER_H
