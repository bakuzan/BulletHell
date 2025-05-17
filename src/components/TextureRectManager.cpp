#include "constants/Constants.h"
#include "TextureRectManager.h"

TextureRectManager::TextureRectManager()
{
    initEnemyTextureRects();
    initProjectileTextureRects();
    initUpgradeBoxTextureRects();
}

TextureRectManager::~TextureRectManager()
{
    // Destructor
}

// Publics

const sf::IntRect &TextureRectManager::getTextureRect(EnemyType type) const
{
    return enemyTextureRects.at(type);
}

const sf::IntRect &TextureRectManager::getTextureRect(ProjectileType type) const
{
    return projectileTextureRects.at(type);
}

const sf::IntRect &TextureRectManager::getTextureRect(UpgradeBoxType type) const
{
    return upgradeBoxTextureRects.at(type);
}

const std::unordered_map<UpgradeBoxType, sf::IntRect> &TextureRectManager::getUpgradeBoxTextureRects() const
{
    return upgradeBoxTextureRects;
}

// Privates

void TextureRectManager::initEnemyTextureRects()
{
    enemyTextureRects[EnemyType::BASIC] = sf::IntRect(520, Constants::SPRITE_OFFSET_Y, Constants::SPRITE_WIDTH_BASIC, Constants::SPRITE_HEIGHT_BASIC);
    enemyTextureRects[EnemyType::SHOOTER] = sf::IntRect(256, 128, Constants::SPRITE_WIDTH_SHOOTER, Constants::SPRITE_HEIGHT_SHOOTER);
    enemyTextureRects[EnemyType::SPEEDY] = sf::IntRect(392, 136, Constants::SPRITE_WIDTH_SPEEDY, Constants::SPRITE_HEIGHT_SPEEDY);
    enemyTextureRects[EnemyType::BOMBER] = sf::IntRect(256, Constants::SPRITE_OFFSET_Y, Constants::SPRITE_WIDTH_BOMBER, Constants::SPRITE_HEIGHT_BOMBER);

    enemyTextureRects[EnemyType::BOSS] = sf::IntRect(128, 136, Constants::SPRITE_WIDTH_BOSS, Constants::SPRITE_HEIGHT_BOSS);
}

void TextureRectManager::initProjectileTextureRects()
{
    // PLAYER
    projectileTextureRects[ProjectileType::BULLET] = sf::IntRect(100, 0, 300, 500);
    projectileTextureRects[ProjectileType::LASER] = sf::IntRect(1100, 0, 300, 500);
    projectileTextureRects[ProjectileType::MISSILE] = sf::IntRect(600, 0, 300, 500);
    projectileTextureRects[ProjectileType::MISSILE_DEBRIS] = sf::IntRect(700, 0, 100, 100);

    // ENEMY
    projectileTextureRects[ProjectileType::ALIEN_BULLET] = sf::IntRect(100, 500, 300, 200);
    projectileTextureRects[ProjectileType::ALIEN_DEBRIS] = sf::IntRect(200, 500, 100, 100);
    projectileTextureRects[ProjectileType::ALIEN_LASER] = sf::IntRect(1100, 500, 300, 500);
    projectileTextureRects[ProjectileType::ALIEN_SEEKER] = sf::IntRect(600, 700, 300, 300);
}

void TextureRectManager::initUpgradeBoxTextureRects()
{
    float size = 64;
    upgradeBoxTextureRects[UpgradeBoxType::HEALTH] = sf::IntRect(0 * size, 0, size, size);
    upgradeBoxTextureRects[UpgradeBoxType::DOUBLE_SHOT] = sf::IntRect(1 * size, 0, size, size);
    upgradeBoxTextureRects[UpgradeBoxType::LASER] = sf::IntRect(2 * size, 0, size, size);
    upgradeBoxTextureRects[UpgradeBoxType::MISSILE] = sf::IntRect(3 * size, 0, size, size);
}