#include "ProjectileTextureManager.h"

ProjectileTextureManager::ProjectileTextureManager()
{
    textureRects[ProjectileType::BULLET] = sf::IntRect(0, 0, 500, 500);
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