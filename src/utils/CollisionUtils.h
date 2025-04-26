#ifndef COLLISIONUTILS_H
#define COLLISIONUTILS_H

#include <SFML/Graphics.hpp>
#include <vector>

namespace CollisionUtils
{
    std::vector<sf::Vector2f> GetTransformedVertices(const sf::Sprite &sprite);
    bool CheckSpritesIntersect(const sf::Sprite &spriteA, const sf::Sprite &spriteB);
}

#endif // COLLISIONUTILS_H
