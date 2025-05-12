#pragma once

#include <SFML/Graphics.hpp>

#include "constants/ProjectileType.h"
#include "data/ProjectileData.h"
#include "data/SpawnData.h"
#include "data/WeaponAttributes.h"

struct ProjectileData
{
    ProjectileType type;
    sf::Vector2f position;  // Starting position
    sf::Vector2f direction; // Direction of travel (normalized)
    float damage;
    float speed;
    int debrisCircleProjectileCount; // Number of debris created for chained projectile
    ProjectileAttributes projectileAttrs;

    static ProjectileData CreateChained(ProjectileType type,
                                        sf::Vector2f position,
                                        float damage, float speed,
                                        int debrisCount)
    {
        return {type,
                position,
                {0, 0},
                damage,
                speed,
                debrisCount,
                {0.0f, 0.0f}};
    }

    static ProjectileData CreateRegular(WeaponAttributes weaponAttrs,
                                        SpawnData spawnData)
    {
        return {weaponAttrs.projectileType,
                spawnData.position,
                spawnData.velocity,
                weaponAttrs.damage,
                weaponAttrs.speed,
                0,
                weaponAttrs.projectileAttrs};
    }
};
