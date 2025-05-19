#include "Enemy.h"

Enemy::Enemy(EnemyType t,
             const sf::Texture &texture, sf::IntRect textureRect,
             sf::Vector2f spawnPosition,
             EnemyStats enemyStats)
    : type(t), initialStats(enemyStats), stats(enemyStats)
{
    sprite.setTexture(texture);
    sprite.setTextureRect(textureRect);
    sprite.setPosition(spawnPosition);
}

Enemy::~Enemy()
{
    // Destructor
}

// Publics

void Enemy::render(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

const EnemyType Enemy::getType() const
{
    return type;
}

const float Enemy::getHealth() const
{
    return stats.health;
}

const sf::Sprite &Enemy::getSprite() const
{
    return sprite;
}

const int Enemy::getPointsValue() const
{
    return stats.pointValue;
}

void Enemy::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}

void Enemy::updateHealth(float adjustment)
{
    stats.health = std::max(0.0f, stats.health + adjustment);
}