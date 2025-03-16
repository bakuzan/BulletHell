#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

class Projectile
{
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;

public:
    Projectile(float x, float y, float width, float height, sf::Vector2f velocity);
    ~Projectile();

    void update(sf::Time deltaTime);
    void render(sf::RenderWindow &window) const;
    bool isOffScreen(sf::RenderWindow &window);
};

#endif // PROJECTILE_H
