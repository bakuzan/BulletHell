#include <cmath>
#include <iomanip>
#include <sstream>

#include "constants/Constants.h"
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
        float angle = angleRadians * (180.f / M_PI); // Convert to degrees

        sprite.setRotation(angle + alignmentOffset); // Adjust for sprite alignment

        return angleRadians;
    }

    SpawnData getSpawnDataForProjectileFromEntity(
        const sf::Sprite &sprite,
        const WeaponAttributes &weaponAttrs,
        float rotationOffset)
    {
        // Get current rotation and convert to radians
        float angleDegrees = sprite.getRotation() - rotationOffset;
        float angleRadians = angleDegrees * (M_PI / 180.0f);

        // Calculate the bullet's velocity based on the entity's rotation
        sf::Vector2f direction(std::cos(angleRadians), std::sin(angleRadians));
        sf::Vector2f bulletVelocity = direction * weaponAttrs.speed;

        // Calculate bullet spawn position (front-center of the entity)
        float bulletWidth = weaponAttrs.projectileAttrs.width;
        float bulletHeight = weaponAttrs.projectileAttrs.height;
        float entityWidth = sprite.getGlobalBounds().width;
        float entityHeight = sprite.getGlobalBounds().height;

        sf::Vector2f rotatedCenterOffset = sf::Vector2f(
            -entityWidth / 2.f * std::cos(angleRadians) + entityHeight / 2.f * std::sin(angleRadians),
            -entityWidth / 2.f * std::sin(angleRadians) - entityHeight / 2.f * std::cos(angleRadians));

        // Offset to the front + center
        sf::Vector2f verticalOffset = direction * ((entityHeight / 2.f) + bulletHeight);
        sf::Vector2f horizontalOffset = sf::Vector2f(
            -bulletWidth / 2.f * direction.y,
            bulletWidth / 2.f * direction.x);

        // Calculate final spawn position
        sf::Vector2f spawnPosition = sprite.getPosition() +
                                     rotatedCenterOffset +
                                     verticalOffset +
                                     horizontalOffset;

        return {spawnPosition, bulletVelocity};
    }

    const sf::Vector2f getRandomPositionOnSide(
        const sf::View &view,
        Direction side,
        float offset)
    {
        sf::Vector2f viewSize = view.getSize();
        sf::Vector2f viewCenter = view.getCenter();

        switch (side)
        {
        case Direction::UP:
            return {viewCenter.x - viewSize.x / 2.f + static_cast<float>(rand() % int(viewSize.x)),
                    viewCenter.y - viewSize.y / 2.f - offset};
        case Direction::LEFT:
            return {viewCenter.x - viewSize.x / 2.f - offset,
                    viewCenter.y - viewSize.y / 2.f + static_cast<float>(rand() % int(viewSize.y))};
        case Direction::RIGHT:
            return {viewCenter.x + viewSize.x / 2.f + offset,
                    viewCenter.y - viewSize.y / 2.f + static_cast<float>(rand() % int(viewSize.y))};
        case Direction::DOWN:
            return {viewCenter.x - viewSize.x / 2.f + static_cast<float>(rand() % int(viewSize.x)),
                    viewCenter.y + viewSize.y / 2.f + offset};
        case Direction::NONE:
        default:
            return {0.f, 0.f}; // Fallback (this should never occur)
        }
    }

    std::string formatScoreText(int score)
    {
        std::stringstream ss;
        ss << std::setw(8) << std::setfill('0') << score;
        return ss.str();
    }

}