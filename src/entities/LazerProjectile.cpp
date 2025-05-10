#include <cmath>
#include <iostream>

#include "LazerProjectile.h"
#include "utils/GameUtils.h"

LazerProjectile::LazerProjectile(ProjectileType type,
                                 const sf::Texture &texture, sf::IntRect textureRect,
                                 sf::Vector2f spawnPosition, sf::Vector2f velocity,
                                 float damage)
    : Projectile(type,
                 texture, textureRect,
                 spawnPosition, velocity,
                 damage),
      fadeDuration(0.5f),
      fadeOutTimeRemaining(0.5f),
      hasDamageBeenCalculated(false)
{
    sprite.setScale(0.05f, 10.0f);
}

LazerProjectile::~LazerProjectile()
{
    // Destructor
}

// Publics
void LazerProjectile::update(sf::Time deltaTime)
{
    if (fadeOutTimeRemaining > 0.0f)
    {
        fadeOutTimeRemaining -= deltaTime.asSeconds();
        float alpha = 255 * (fadeOutTimeRemaining / fadeDuration);
        sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
    }

    // Ensure lazer is angled correctly
    float angle = std::atan2(velocity.y, velocity.x) * 180.f / M_PI; // Convert radians to degrees
    sprite.setRotation(angle + 90.0f);                               // Add offset due to position in the spritesheet
}

const sf::Vector2f &LazerProjectile::getStartPoint() const
{
    return sprite.getPosition();
}

const sf::Vector2f LazerProjectile::getEndPoint() const
{
    sf::Vector2f startPos = getStartPoint();
    sf::Vector2f direction = GameUtils::normaliseVector(velocity);
    float range = sprite.getGlobalBounds().height;

    return startPos + (direction * range);
}

const bool LazerProjectile::canBeRemoved() const
{
    return fadeOutTimeRemaining <= 0.0f;
}

const bool LazerProjectile::isDamageCalculated() const
{
    return hasDamageBeenCalculated;
}

void LazerProjectile::setDamageCalculated(bool isCalculated)
{
    hasDamageBeenCalculated = isCalculated;
}
