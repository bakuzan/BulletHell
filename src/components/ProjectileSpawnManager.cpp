#include <cmath>

#include "ProjectileSpawnManager.h"
#include "constants/Constants.h"
#include "entities/LaserProjectile.h"
#include "entities/SeekerProjectile.h"

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
        float playerRotationRadians = playerRotation * M_PI / 180.0f; // Convert to radians

        float offsetX = spacing * std::cos(playerRotationRadians);
        float offsetY = spacing * std::sin(playerRotationRadians);

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

void ProjectileSpawnManager::spawnChainedProjectiles(
    const sf::Texture &texture,
    std::vector<std::unique_ptr<Projectile>> &projectiles,
    const std::vector<ProjectileData> &projectileData)
{
    for (const auto &data : projectileData)
    {
        for (int i = 0; i < data.debrisCircleProjectileCount; ++i)
        {
            float angle = (2 * M_PI / data.debrisCircleProjectileCount) * i; // Angle (radians)
            sf::Vector2f direction(std::cos(angle), std::sin(angle));

            ProjectileData newData = data;
            newData.direction = direction * data.speed;

            projectiles.emplace_back(spawnProjectile(texture, newData));
        }
    }
}

// Privates

std::unique_ptr<Projectile> ProjectileSpawnManager::spawnProjectile(
    const sf::Texture &texture,
    const ProjectileData &data)
{
    switch (data.type)
    {
    case ProjectileType::LASER:
    case ProjectileType::ALIEN_LASER:
        return std::make_unique<LaserProjectile>(data.type,
                                                 texture,
                                                 projectileTextureManager.getTextureRect(data.type),
                                                 data.position,
                                                 data.direction,
                                                 data.damage);
    case ProjectileType::ALIEN_SEEKER:
        return std::make_unique<SeekerProjectile>(data.type,
                                                  texture,
                                                  projectileTextureManager.getTextureRect(data.type),
                                                  data.position,
                                                  data.direction,
                                                  data.damage,
                                                  data.speed);
    case ProjectileType::BULLET:
    case ProjectileType::MISSILE:
    case ProjectileType::MISSILE_DEBRIS:
    case ProjectileType::ALIEN_BULLET:
    case ProjectileType::ALIEN_DEBRIS:
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