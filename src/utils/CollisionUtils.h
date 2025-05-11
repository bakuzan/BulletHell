#ifndef COLLISIONUTILS_H
#define COLLISIONUTILS_H

#include <SFML/Graphics.hpp>
#include <vector>

namespace CollisionUtils
{
    std::vector<sf::Vector2f> GetTransformedVertices(const sf::Sprite &sprite);
    bool CheckSpritesIntersect(const sf::Sprite &spriteA, const sf::Sprite &spriteB);

    std::pair<int, int> getGridCell(int gridCellSize,
                                    const sf::Vector2f &position);
    std::vector<std::pair<int, int>> getLaserCells(int gridCellSize,
                                                   const sf::Vector2f &start,
                                                   const sf::Vector2f &end);
}

#endif // COLLISIONUTILS_H
