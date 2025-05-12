#ifndef LAZERPROJECTILE_H
#define LAZERPROJECTILE_H

#include "Projectile.h"
#include "data/ProjectileAttributes.h"

class LaserProjectile : public Projectile
{
private:
    float fadeDuration;
    float fadeOutTimeRemaining;
    bool hasDamageBeenCalculated;

public:
    static constexpr float rotationOffset = 90.0f;

public:
    LaserProjectile(ProjectileType type,
                    const sf::Texture &texture, sf::IntRect textureRect,
                    sf::Vector2f spawnPosition, sf::Vector2f velocity,
                    float damage,
                    ProjectileAttributes projectileAttrs);
    ~LaserProjectile();

    void update(sf::Time deltaTime) override;

    const sf::Vector2f getStartPoint() const;
    const sf::Vector2f getEndPoint() const;

    const bool canBeRemoved() const;
    const bool isDamageCalculated() const;
    void setDamageCalculated(bool isCalculated);
};

#endif // LAZERPROJECTILE_H
