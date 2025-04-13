#ifndef UPGRADEBOXTEXTUREMANAGER_H
#define UPGRADEBOXTEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "UpgradeBoxType.h"

class UpgradeBoxTextureManager
{
private:
    std::unordered_map<UpgradeBoxType, sf::IntRect> textureRects;

public:
    UpgradeBoxTextureManager();
    ~UpgradeBoxTextureManager();

    const sf::IntRect &getTextureRect(UpgradeBoxType type) const;
};

#endif // UPGRADEBOXTEXTUREMANAGER_H
