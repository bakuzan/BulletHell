#ifndef LAZERPROJECTILE_H
#define LAZERPROJECTILE_H

#include "Projectile.h"

class LazerProjectile : public Projectile
{
public:
    LazerProjectile(const sf::Texture &texture, sf::IntRect textureRect,
                    sf::Vector2f spawnPosition, sf::Vector2f velocity,
                    float damage);
    ~LazerProjectile();

    void update(sf::Time deltaTime) override;

private:
};

#endif // LAZERPROJECTILE_H
