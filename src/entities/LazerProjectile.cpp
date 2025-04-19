#include "LazerProjectile.h"

LazerProjectile::LazerProjectile(const sf::Texture &texture, sf::IntRect textureRect,
                                 sf::Vector2f spawnPosition, sf::Vector2f velocity,
                                 float damage)
    : Projectile(ProjectileType::LAZER,
                 texture, textureRect,
                 spawnPosition, velocity,
                 damage)
{
    sprite.setScale(1.0f, 1.0f);
}

LazerProjectile::~LazerProjectile()
{
    // Destructor
}

// Publics
void LazerProjectile::update(sf::Time deltaTime)
{
    sprite.move(velocity * deltaTime.asSeconds());
}