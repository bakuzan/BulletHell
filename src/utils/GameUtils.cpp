#include <cmath>
#include <iomanip>
#include <sstream>

#include "GameUtils.h"

namespace GameUtils
{
    sf::Vector2f normaliseVector(const sf::Vector2f &vector)
    {
        float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);
        if (magnitude > 0)
        {
            return vector / magnitude;
        }

        return sf::Vector2f(0.0f, 0.0f);
    }

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

    std::string formatScoreText(int score)
    {
        std::stringstream ss;
        ss << std::setw(8) << std::setfill('0') << score;
        return ss.str();
    }

}