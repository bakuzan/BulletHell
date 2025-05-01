#include <cmath>

#include "ShooterEnemy.h"
#include "constants/Constants.h"
#include "constants/ProjectileType.h"
#include "utils/GameUtils.h"
#include "components/WeaponAttributesManager.h"

ShooterEnemy::ShooterEnemy(
    const sf::Texture &texture, sf::IntRect textureRect,
    sf::Vector2f spawnPosition, float movementSpeed)
    : Enemy(EnemyType::SHOOTER,
            texture, textureRect,
            spawnPosition,
            movementSpeed, Constants::ENEMY_POINTS_SHOOTER, Constants::ENEMY_HEALTH_SHOOTER)
{
    sprite.setOrigin(Constants::SPRITE_WIDTH_SHOOTER / 2.0f, Constants::SPRITE_HEIGHT_SHOOTER / 2.0f);
    sprite.setScale(0.5, 0.5);
}

ShooterEnemy::~ShooterEnemy()
{
    // Destructor
}

// Publics
void ShooterEnemy::update(float deltaTime, const sf::Vector2f &playerPosition)
{
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    float magnitude = calculateDistanceToPlayerMagnitude(playerPosition);

    if (magnitude > 0)
    {
        direction /= magnitude; // Normalize the vector
    }

    if (magnitude > Constants::ENEMY_SHOOTER_DISTANCE)
    {
        sprite.move(direction * speed * deltaTime);
    }

    GameUtils::rotateTowards(
        sprite,
        sprite.getPosition(),
        playerPosition,
        rotationOffset);
}

std::optional<ProjectileData> ShooterEnemy::getShootData(
    float deltaTime,
    const sf::Vector2f &playerPosition)
{
    if (shouldShoot(deltaTime, playerPosition))
    {
        WeaponAttributes weaponAttrs = WeaponAttributesManager::getInstance().getAttributes(WeaponType::ALIEN_BASIC);

        SpawnData projectileSpawnData =
            GameUtils::getSpawnDataForProjectileFromEntity(
                sprite,
                weaponAttrs,
                rotationOffset);

        return ProjectileData::CreateRegular(
            weaponAttrs.projectileType,
            projectileSpawnData.position,
            projectileSpawnData.velocity,
            weaponAttrs.damage);
    }

    return std::nullopt;
}

// Privates

float ShooterEnemy::calculateDistanceToPlayerMagnitude(const sf::Vector2f &playerPosition)
{
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    return std::sqrt(direction.x * direction.x + direction.y * direction.y);
}

bool ShooterEnemy::shouldShoot(float deltaTime,
                               const sf::Vector2f &playerPosition)
{
    static float shootCooldown = 1.0f;
    static float timeSinceLastShot = 0.0f;

    float magnitude = calculateDistanceToPlayerMagnitude(playerPosition);
    if (magnitude <= Constants::ENEMY_SHOOTER_DISTANCE)
    {
        timeSinceLastShot += deltaTime;

        if (timeSinceLastShot >= shootCooldown)
        {
            timeSinceLastShot -= shootCooldown;
            return true;
        }
    }

    return false;
}