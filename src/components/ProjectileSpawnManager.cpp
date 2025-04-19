#include <cmath>

#include "ProjectileSpawnManager.h"
#include "constants/Constants.h"
#include "entities/LazerProjectile.h"

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
    std::vector<std::unique_ptr<Projectile>> &projectiles,
    ProjectileData &data,
    float playerRotation)
{
    if (data.type == ProjectileType::DOUBLE_SHOT)
    {
        // Double-shot is a "fake" type that simply denotes shooting two bullets side-by-side
        data.type = ProjectileType::BULLET;

        float spacing = 10.0f;
        float playerRotationRadians = (playerRotation - 90.0f) * M_PI / 180.0f; // Convert to radians
        float angleOffset = M_PI / 2;                                           // 90 degrees in radians (perpendicular to the player's rotation)

        float offsetX = spacing * std::cos(playerRotationRadians + angleOffset);
        float offsetY = spacing * std::sin(playerRotationRadians + angleOffset);

        // Left bullet
        data.position.x -= offsetX;
        data.position.y -= offsetY;
        projectiles.emplace_back(spawnProjectile(texture, data));

        // Right bullet
        data.position.x += 2 * offsetX;
        data.position.y += 2 * offsetY;
        projectiles.emplace_back(spawnProjectile(texture, data));
    }
    else
    {
        projectiles.emplace_back(spawnProjectile(texture, data));
    }
}

void ProjectileSpawnManager::spawnEnemyProjectile(
    const sf::Texture &texture,
    std::vector<std::unique_ptr<Projectile>> &projectiles,
    ProjectileData &data)
{
    projectiles.emplace_back(spawnProjectile(texture, data));
}

// Privates

std::unique_ptr<Projectile> ProjectileSpawnManager::spawnProjectile(
    const sf::Texture &texture,
    ProjectileData &data)
{
    switch (data.type)
    {
    case ProjectileType::LAZER:
        return std::make_unique<LazerProjectile>(texture,
                                                 projectileTextureManager.getTextureRect(data.type),
                                                 data.position,
                                                 data.direction,
                                                 data.damage);
    case ProjectileType::BULLET:
    case ProjectileType::MISSILE:
    case ProjectileType::BULLET_ALIEN:
        return std::make_unique<Projectile>(data.type,
                                            texture,
                                            projectileTextureManager.getTextureRect(data.type),
                                            data.position,
                                            data.direction,
                                            data.damage);
    case ProjectileType::DOUBLE_SHOT:
    default:
        throw std::logic_error("Unhandled ProjectileType in ProjectileSpawnManager::spawnProjectile()");
    }
}