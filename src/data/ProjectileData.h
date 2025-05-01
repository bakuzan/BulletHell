#pragma once

#include <SFML/Graphics.hpp>

#include "constants/ProjectileType.h"

struct ProjectileData
{
    ProjectileType type;
    sf::Vector2f position;  // Starting position
    sf::Vector2f direction; // Direction of travel (normalized)
    float damage;
    float speed;
    int debrisCircleProjectileCount; // Number of debris created for chained projectile

    static ProjectileData CreateChained(ProjectileType type, sf::Vector2f position, float damage, float speed, int debrisCount)
    {
        return {type, position, {0, 0}, damage, speed, debrisCount};
    }

    static ProjectileData CreateRegular(ProjectileType type, sf::Vector2f position, sf::Vector2f direction, float damage)
    {
        return {type, position, direction, damage, 0.0f, 0};
    }
};
