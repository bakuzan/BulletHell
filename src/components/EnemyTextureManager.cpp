#include "constants/Constants.h"
#include "EnemyTextureManager.h"

EnemyTextureManager::EnemyTextureManager()
{
    textureRects[EnemyType::BASIC] = sf::IntRect(520, Constants::SPRITE_OFFSET_Y, Constants::SPRITE_WIDTH_BASIC, Constants::SPRITE_HEIGHT_BASIC);
    textureRects[EnemyType::SHOOTER] = sf::IntRect(256, 128, Constants::SPRITE_WIDTH_SHOOTER, Constants::SPRITE_HEIGHT_SHOOTER);
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