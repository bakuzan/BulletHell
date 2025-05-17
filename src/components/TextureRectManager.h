#ifndef TEXTURERECTMANAGER_H
#define TEXTURERECTMANAGER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "constants/EnemyType.h"
#include "constants/ProjectileType.h"
#include "constants/UpgradeBoxType.h"

class TextureRectManager
{
private:
    std::unordered_map<EnemyType, sf::IntRect> enemyTextureRects;
    std::unordered_map<ProjectileType, sf::IntRect> projectileTextureRects;
    std::unordered_map<UpgradeBoxType, sf::IntRect> upgradeBoxTextureRects;

private:
    void initEnemyTextureRects();
    void initProjectileTextureRects();
    void initUpgradeBoxTextureRects();

public:
    TextureRectManager();
    ~TextureRectManager();

    const sf::IntRect &getTextureRect(EnemyType type) const;
    const sf::IntRect &getTextureRect(ProjectileType type) const;
    const sf::IntRect &getTextureRect(UpgradeBoxType type) const;

    const std::unordered_map<UpgradeBoxType, sf::IntRect> &getUpgradeBoxTextureRects() const;
};

#endif // TEXTURERECTMANAGER_H
