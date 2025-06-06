#include <cmath>
#include <limits>

#include "CollisionUtils.h"

namespace CollisionUtils
{

    std::vector<sf::Vector2f> GetTransformedVertices(const sf::Sprite &sprite)
    {
        sf::FloatRect localBounds = sprite.getLocalBounds(); // Original untransformed bounds
        sf::Transform transform = sprite.getTransform();     // Applies position, rotation, scale

        std::vector<sf::Vector2f> vertices = {
            transform.transformPoint(localBounds.left, localBounds.top),
            transform.transformPoint(localBounds.left + localBounds.width, localBounds.top),
            transform.transformPoint(localBounds.left + localBounds.width, localBounds.top + localBounds.height),
            transform.transformPoint(localBounds.left, localBounds.top + localBounds.height)};

        return vertices;
    }

    bool CheckSpritesIntersect(const sf::Sprite &spriteA, const sf::Sprite &spriteB)
    {
        // Get the transformed vertices of the sprites
        std::vector<sf::Vector2f> verticesA = GetTransformedVertices(spriteA);
        std::vector<sf::Vector2f> verticesB = GetTransformedVertices(spriteB);

        // Combine both sets of vertices for edge-based axes
        for (const std::vector<sf::Vector2f> &vertices : {verticesA, verticesB})
        {
            for (size_t i1 = 0; i1 < vertices.size(); ++i1)
            {
                size_t i2 = (i1 + 1) % vertices.size();
                sf::Vector2f p1 = vertices[i1];
                sf::Vector2f p2 = vertices[i2];

                // Calculate the perpendicular axis (normal to the edge)
                sf::Vector2f normal(p2.y - p1.y, p1.x - p2.x);

                // Project both polygons onto this axis
                auto project = [](const std::vector<sf::Vector2f> &vertices, const sf::Vector2f &axis)
                {
                    float min = std::numeric_limits<float>::max();
                    float max = std::numeric_limits<float>::lowest();

                    for (const sf::Vector2f &vertex : vertices)
                    {
                        float projection = vertex.x * axis.x + vertex.y * axis.y; // Dot product
                        min = std::min(min, projection);
                        max = std::max(max, projection);
                    }

                    return std::make_pair(min, max);
                };

                auto [minA, maxA] = project(verticesA, normal);
                auto [minB, maxB] = project(verticesB, normal);

                // If no overlap on this axis, the polygons don't intersect
                if (maxA < minB || maxB < minA)
                {
                    return false;
                }
            }
        }

        return true; // Polygons intersect on all axes
    }

    std::pair<int, int> getGridCell(int gridCellSize,
                                    const sf::Vector2f &position)
    {
        return std::make_pair(static_cast<int>(position.x) / gridCellSize,
                              static_cast<int>(position.y) / gridCellSize);
    }

    std::vector<std::pair<int, int>> getLaserCells(int gridCellSize,
                                                   const sf::Vector2f &start,
                                                   const sf::Vector2f &end)
    {
        std::vector<std::pair<int, int>> cells;

        float dx = end.x - start.x;
        float dy = end.y - start.y;
        int steps = static_cast<int>(std::ceil(std::hypot(dx, dy) / gridCellSize));
        float stepSize = 1.0f / steps;

        for (float t = 0; t <= steps; t += stepSize)
        {
            sf::Vector2f point = start + t * (end - start);
            cells.emplace_back(static_cast<int>(point.x) / gridCellSize,
                               static_cast<int>(point.y) / gridCellSize);
        }

        return cells;
    }

}