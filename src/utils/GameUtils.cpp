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
        sf::Vector2f firingEntityPos = sprite.getPosition();

        float projectileWidth = weaponAttrs.projectileAttrs.width;
        float projectileHeight = weaponAttrs.projectileAttrs.height;
        float firingEntityHeight = sprite.getGlobalBounds().height;

        // Using direction, move projectile outwards
        sf::Vector2f pushOutOffset = direction * ((firingEntityHeight / 2.f) + projectileHeight);

        // Using perpendicular to direction, move projectile across
        sf::Vector2f perpendicularVector(-direction.y, direction.x);
        sf::Vector2f lateralOffset = perpendicularVector * -(projectileWidth / 2.f);

        // Calculate final spawn position
        sf::Vector2f spawnPosition = firingEntityPos +
                                     pushOutOffset +
                                     lateralOffset;

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

    const AudioId getAudioIdForProjectileType(ProjectileType projectileType)
    {
        switch (projectileType)
        {
        case ProjectileType::BULLET:
        case ProjectileType::DOUBLE_SHOT:
            return AudioId::PROJECTILE_BASIC;
        case ProjectileType::LASER:
            return AudioId::PROJECTILE_LASER;
        case ProjectileType::MISSILE:
            return AudioId::PROJECTILE_MISSILE;
        case ProjectileType::MISSILE_DEBRIS:
        case ProjectileType::ALIEN_DEBRIS:
            return AudioId::PROJECTILE_EXPLOSION_DEBRIS;

        case ProjectileType::ALIEN_BULLET:
            return AudioId::PROJECTILE_ALIEN_BASIC;
        case ProjectileType::ALIEN_LASER:
            return AudioId::PROJECTILE_LASER;
        case ProjectileType::ALIEN_SEEKER:
            return AudioId::PROJECTILE_MISSILE;
        }

        throw std::logic_error("Unhandled ProjectileType in GameUtils::getAudioIdForProjectileType()");
    }

    const WeaponType mapUpgradeBoxToWeapon(UpgradeBoxType upgradeType)
    {
        switch (upgradeType)
        {
        case UpgradeBoxType::DOUBLE_SHOT:
            return WeaponType::DOUBLE_SHOT;
        case UpgradeBoxType::LASER:
            return WeaponType::LASER;
        case UpgradeBoxType::MISSILE:
            return WeaponType::MISSILE;
        case UpgradeBoxType::HEALTH:
        case UpgradeBoxType::COUNT:
        default:
            return WeaponType::BASIC; // Shouldn't hit!
        }
    }

    const UpgradeBoxType getUpgradeBoxTypeForWeaponType(WeaponType weaponType)
    {
        switch (weaponType)
        {
        case WeaponType::DOUBLE_SHOT:
            return UpgradeBoxType::DOUBLE_SHOT;
        case WeaponType::MISSILE:
            return UpgradeBoxType::MISSILE;
        case WeaponType::LASER:
            return UpgradeBoxType::LASER;

        case WeaponType::BASIC:
        case WeaponType::ALIEN_BASIC:
        case WeaponType::ALIEN_LASER:
        case WeaponType::ALIEN_SEEKER:
            throw std::logic_error("Invalid WeaponType in GameUtils::getUpgradeBoxTypeForWeaponType()");
        }

        throw std::logic_error("Unhandled WeaponType in GameUtils::getUpgradeBoxTypeForWeaponType()");
    }

    std::string formatScoreText(int score)
    {
        std::stringstream ss;
        ss << std::setw(8) << std::setfill('0') << score;
        return ss.str();
    }

}