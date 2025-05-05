#include "ProjectileTextureManager.h"

ProjectileTextureManager::ProjectileTextureManager()
{
    // PLAYER
    textureRects[ProjectileType::BULLET] = sf::IntRect(100, 0, 300, 500);
    textureRects[ProjectileType::LAZER] = sf::IntRect(1100, 0, 300, 500);
    textureRects[ProjectileType::MISSILE] = sf::IntRect(600, 0, 300, 500);
    textureRects[ProjectileType::MISSILE_DEBRIS] = sf::IntRect(700, 0, 100, 100);

    // ENEMY
    textureRects[ProjectileType::ALIEN_BULLET] = sf::IntRect(100, 500, 300, 200);
    textureRects[ProjectileType::ALIEN_DEBRIS] = sf::IntRect(200, 500, 100, 100);
    textureRects[ProjectileType::ALIEN_LAZER] = sf::IntRect(1100, 500, 300, 500);
    textureRects[ProjectileType::ALIEN_SEEKER] = sf::IntRect(600, 700, 300, 300);
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