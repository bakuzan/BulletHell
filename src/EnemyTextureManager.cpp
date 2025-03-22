#include "EnemyTextureManager.h"

EnemyTextureManager::EnemyTextureManager()
{
    textureRects[EnemyType::BASIC] = sf::IntRect(0, 0, 500, 500);
}

EnemyTextureManager::~EnemyTextureManager()
{
    // Destructor
}

// Publics
const sf::IntRect &EnemyTextureManager::getTextureRect(EnemyType type) const
{
    return textureRects.at(type);
}