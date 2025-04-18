#include "ProjectileSpawnManager.h"
#include "constants/Constants.h"

ProjectileSpawnManager::ProjectileSpawnManager()
{
    // Constructor
}

ProjectileSpawnManager::~ProjectileSpawnManager()
{
    // Destructor
}

// Publics

void ProjectileSpawnManager::spawnPlayerProjectile(
    const sf::Texture &texture,
    std::vector<Projectile> &projectiles,
    ProjectileData &data)
{
    projectiles.emplace_back(data.type,
                             texture,
                             projectileTextureManager.getTextureRect(data.type),
                             data.position,
                             data.direction,
                             data.damage);
}

void ProjectileSpawnManager::spawnEnemyProjectile(
    const sf::Texture &texture,
    std::vector<Projectile> &projectiles,
    ProjectileData &data)
{
    projectiles.emplace_back(data.type,
                             texture,
                             projectileTextureManager.getTextureRect(data.type),
                             data.position,
                             data.direction,
                             data.damage);
}
