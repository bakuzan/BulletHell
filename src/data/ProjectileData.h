#pragma once

#include <SFML/Graphics.hpp>

#include "constants/ProjectileType.h"

struct ProjectileData
{
    ProjectileType type;
    sf::Vector2f position;  // Starting position
    sf::Vector2f direction; // Direction of travel (normalized)
    float damage;

    ProjectileData(ProjectileType typ, sf::Vector2f pos, sf::Vector2f dir, float dmg)
        : type(typ), position(pos), direction(dir), damage(dmg)
    {
    }
};
