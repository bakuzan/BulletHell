#pragma once

#include <SFML/Graphics.hpp>

struct ProjectileData
{
    sf::Vector2f position;  // Starting position
    sf::Vector2f direction; // Direction of travel (normalized)
    float damage;

    ProjectileData(sf::Vector2f pos, sf::Vector2f dir, float dmg)
        : position(pos), direction(dir), damage(dmg)
    {
    }
};
