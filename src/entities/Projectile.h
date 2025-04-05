#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

#include "ProjectileType.h"

class Projectile
{
private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    ProjectileType type;

public:
    Projectile(ProjectileType type,
               const sf::Texture &texture, sf::IntRect textureRect,
               sf::Vector2f spawnPosition, sf::Vector2f velocity);
    ~Projectile();

    void update(sf::Time deltaTime);
    void render(sf::RenderWindow &window) const;

    const sf::Sprite &getSprite() const;
    bool isOffScreen(sf::RenderWindow &window);
};

#endif // PROJECTILE_H
