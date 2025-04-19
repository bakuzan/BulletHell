#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

#include "constants/ProjectileOrigin.h"
#include "constants/ProjectileType.h"

class Projectile
{
protected:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    ProjectileType type;
    float damage;

public:
    Projectile(ProjectileType type,
               const sf::Texture &texture, sf::IntRect textureRect,
               sf::Vector2f spawnPosition, sf::Vector2f velocity,
               float damage);
    virtual ~Projectile() = default;

    virtual void update(sf::Time deltaTime);
    void render(sf::RenderWindow &window) const;

    const ProjectileOrigin getOrigin() const;
    const float getDamageInflicts() const;
    const sf::Sprite &getSprite() const;
    bool isOffScreen(sf::RenderWindow &window);
};

#endif // PROJECTILE_H
