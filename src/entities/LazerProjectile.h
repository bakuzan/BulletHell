#ifndef LAZERPROJECTILE_H
#define LAZERPROJECTILE_H

#include "Projectile.h"

class LazerProjectile : public Projectile
{
private:
    float fadeDuration;
    float fadeOutTimeRemaining;
    bool hasDamageBeenCalculated;

public:
    LazerProjectile(const sf::Texture &texture, sf::IntRect textureRect,
                    sf::Vector2f spawnPosition, sf::Vector2f velocity,
                    float damage);
    ~LazerProjectile();

    void update(sf::Time deltaTime) override;

    const bool canBeRemoved() const;
    const bool isDamageCalculated() const;
    void setDamageCalculated(bool isCalculated);
};

#endif // LAZERPROJECTILE_H
