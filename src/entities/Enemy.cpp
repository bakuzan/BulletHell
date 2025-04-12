#include "Enemy.h"

Enemy::Enemy(EnemyType t,
             const sf::Texture &texture, sf::IntRect textureRect,
             sf::Vector2f spawnPosition,
             float movementSpeed, int pointsValue, float startingHealth)
    : type(t), speed(movementSpeed), pointsValue(pointsValue), health(startingHealth)
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
    return health;
}

const sf::Sprite &Enemy::getSprite() const
{
    return sprite;
}

const int Enemy::getPointsValue() const
{
    return pointsValue;
}

void Enemy::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}

void Enemy::updateHealth(float adjustment)
{
    health = std::max(0.0f, health + adjustment);
}