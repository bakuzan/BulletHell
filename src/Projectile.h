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
    Projectile(float x, float y,
               sf::Texture &texture, sf::IntRect textureRect,
               sf::Vector2f velocity, ProjectileType type);
    ~Projectile();

    void update(sf::Time deltaTime);
    void render(sf::RenderWindow &window) const;
    bool isOffScreen(sf::RenderWindow &window);
};

#endif // PROJECTILE_H
