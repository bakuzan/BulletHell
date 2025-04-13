#include "UpgradeBoxTextureManager.h"

UpgradeBoxTextureManager::UpgradeBoxTextureManager()
{
    float size = 64;
    textureRects[UpgradeBoxType::HEALTH] = sf::IntRect(0 * size, 0, size, size);
    textureRects[UpgradeBoxType::DOUBLE_SHOT] = sf::IntRect(1 * size, 0, size, size);
    textureRects[UpgradeBoxType::LAZER] = sf::IntRect(2 * size, 0, size, size);
    textureRects[UpgradeBoxType::MISSLE] = sf::IntRect(3 * size, 0, size, size);
}

UpgradeBoxTextureManager::~UpgradeBoxTextureManager()
{
    // Destructor
}

// Publics
const sf::IntRect &UpgradeBoxTextureManager::getTextureRect(UpgradeBoxType type) const
{
    return textureRects.at(type);
}