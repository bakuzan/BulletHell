#ifndef SEEKERPROJECTILE_H
#define SEEKERPROJECTILE_H

#include "Projectile.h"

class SeekerProjectile : public Projectile
{
private:
    float speed;
    sf::Vector2f targetPosition;

public:
    static constexpr float rotationOffset = 270.0f;

public:
    SeekerProjectile(ProjectileType type,
                     const sf::Texture &texture, sf::IntRect textureRect,
                     sf::Vector2f spawnPosition, sf::Vector2f velocity,
                     float damage, float speed);
    ~SeekerProjectile();

    void update(sf::Time deltaTime) override;
    void setTargetPosition(const sf::Vector2f &position);
};

#endif // SEEKERPROJECTILE_H
