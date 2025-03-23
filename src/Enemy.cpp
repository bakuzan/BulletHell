#include "Enemy.h"

Enemy::Enemy(EnemyType t, const sf::Texture &texture, sf::IntRect textureRect, sf::Vector2f spawnPosition, float movementSpeed)
    : type(t), speed(movementSpeed)
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

const sf::Sprite &Enemy::getSprite() const
{
    return sprite;
}

void Enemy::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}
