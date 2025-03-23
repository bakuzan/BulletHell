#include <cmath>

#include "GameUtils.h"

namespace GameUtils
{
    float rotateTowards(
        sf::Sprite &sprite,
        const sf::Vector2f &sourcePosition,
        const sf::Vector2f &targetPosition,
        float alignmentOffset)
    {
        float dx = targetPosition.x - sourcePosition.x;
        float dy = targetPosition.y - sourcePosition.y;

        float angleRadians = std::atan2(dy, dx);
        float angle = angleRadians * (180.f / 3.14159f); // Convert to degrees

        sprite.setRotation(angle + alignmentOffset); // Adjust for sprite alignment

        return angleRadians;
    }

}