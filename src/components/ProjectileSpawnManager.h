#ifndef PROJECTILESPAWNMANAGER_H
#define PROJECTILESPAWNMANAGER_H

#include <memory>

#include "TextureRectManager.h"

#include "constants/ProjectileType.h"
#include "data/ProjectileData.h"
#include "entities/Projectile.h"

class ProjectileSpawnManager
{
private:
    const TextureRectManager &projectileRectManager;

private:
    std::unique_ptr<Projectile> spawnProjectile(
        const sf::Texture &texture,
        const ProjectileData &data);

public:
    ProjectileSpawnManager(const TextureRectManager &textureRectManager);
    ~ProjectileSpawnManager();

    void spawnPlayerProjectile(const sf::Texture &texture,
                               std::vector<std::unique_ptr<Projectile>> &projectiles,
                               ProjectileData &data,
                               float playerRotation);

    void spawnEnemyProjectile(const sf::Texture &texture,
                              std::vector<std::unique_ptr<Projectile>> &projectiles,
                              ProjectileData &data);

    void spawnChainedProjectiles(const sf::Texture &texture,
                                 std::vector<std::unique_ptr<Projectile>> &projectiles,
                                 const std::vector<ProjectileData> &projectileData);
};

#endif // PROJECTILESPAWNMANAGER_H
