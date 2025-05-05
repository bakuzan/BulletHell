#include "SeekerProjectile.h"
#include "utils/GameUtils.h"

SeekerProjectile::SeekerProjectile(ProjectileType type,
                                   const sf::Texture &texture, sf::IntRect textureRect,
                                   sf::Vector2f spawnPosition, sf::Vector2f velocity,
                                   float damage, float speed)
    : Projectile(type,
                 texture, textureRect,
                 spawnPosition, velocity,
                 damage),
      speed(speed),
      targetPosition(0.0f, 0.0f)
{
}

SeekerProjectile::~SeekerProjectile()
{
    // Destructor
}

// Publics

void SeekerProjectile::update(sf::Time deltaTime)
{
    sf::Vector2f direction = GameUtils::normaliseVector(targetPosition - sprite.getPosition());
    sprite.move(direction * speed * deltaTime.asSeconds());

    GameUtils::rotateTowards(
        sprite,
        sprite.getPosition(),
        targetPosition,
        rotationOffset);
}

void SeekerProjectile::setTargetPosition(const sf::Vector2f &position)
{
    targetPosition = position;
}