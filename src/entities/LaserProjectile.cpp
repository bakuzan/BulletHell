#include <cmath>

#include "LaserProjectile.h"
#include "utils/GameUtils.h"

LaserProjectile::LaserProjectile(ProjectileType type,
                                 const sf::Texture &texture, sf::IntRect textureRect,
                                 sf::Vector2f spawnPosition, sf::Vector2f velocity,
                                 float damage)
    : Projectile(type,
                 texture, textureRect,
                 spawnPosition, velocity,
                 damage),
      fadeDuration(0.3f),
      fadeOutTimeRemaining(0.3f),
      hasDamageBeenCalculated(false)
{
    sprite.setScale(0.05f, 10.0f);
}

LaserProjectile::~LaserProjectile()
{
    // Destructor
}

// Publics
void LaserProjectile::update(sf::Time deltaTime)
{
    if (fadeOutTimeRemaining > 0.0f)
    {
        fadeOutTimeRemaining -= deltaTime.asSeconds();
        float alpha = 255 * (fadeOutTimeRemaining / fadeDuration);
        sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
    }

    // Ensure lazer is angled correctly
    float angle = std::atan2(velocity.y, velocity.x) * 180.f / M_PI; // Convert radians to degrees
    sprite.setRotation(angle + rotationOffset);                      // Add offset due to position in the spritesheet
}

const sf::Vector2f LaserProjectile::getStartPoint() const
{
    sf::Vector2f endPos = getEndPoint();
    sf::Vector2f direction = GameUtils::normaliseVector(velocity);
    float range = 500.0f * 10.0f; // Base length * Scale factor

    return endPos - (direction * range);
}

const sf::Vector2f LaserProjectile::getEndPoint() const
{
    return sprite.getPosition();
}

const bool LaserProjectile::canBeRemoved() const
{
    return fadeOutTimeRemaining <= 0.0f;
}

const bool LaserProjectile::isDamageCalculated() const
{
    return hasDamageBeenCalculated;
}

void LaserProjectile::setDamageCalculated(bool isCalculated)
{
    hasDamageBeenCalculated = isCalculated;
}
