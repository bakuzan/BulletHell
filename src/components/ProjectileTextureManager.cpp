#include "ProjectileTextureManager.h"

ProjectileTextureManager::ProjectileTextureManager()
{
    // PLAYER
    textureRects[ProjectileType::BULLET] = sf::IntRect(100, 0, 300, 500);
    textureRects[ProjectileType::LAZER] = sf::IntRect(1100, 0, 300, 500);
    textureRects[ProjectileType::MISSILE] = sf::IntRect(600, 0, 300, 500);

    // ENEMY
    textureRects[ProjectileType::BULLET_ALIEN] = sf::IntRect(100, 500, 300, 200);
}

ProjectileTextureManager::~ProjectileTextureManager()
{
    // Destructor
}

// Publics
const sf::IntRect &ProjectileTextureManager::getTextureRect(ProjectileType type) const
{
    return textureRects.at(type);
}