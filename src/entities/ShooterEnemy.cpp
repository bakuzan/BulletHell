#include <cmath>

#include "ShooterEnemy.h"
#include "Constants.h"
#include "utils/GameUtils.h"

ShooterEnemy::ShooterEnemy(
    const sf::Texture &texture, sf::IntRect textureRect,
    sf::Vector2f spawnPosition, float movementSpeed)
    : Enemy(EnemyType::SHOOTER, texture, textureRect, spawnPosition, movementSpeed, Constants::ENEMY_POINTS_SHOOTER)
{
    sprite.setScale(0.4, 0.4);
    sprite.setOrigin(Constants::SPRITE_WIDTH_SHOOTER / 2.0f, Constants::SPRITE_HEIGHT_SHOOTER / 2.0f);
}

ShooterEnemy::~ShooterEnemy()
{
    // Destructor
}

// Publics
void ShooterEnemy::update(float deltaTime, const sf::Vector2f &playerPosition)
{
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);

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
        270.0f);
}

std::optional<ProjectileData> ShooterEnemy::getShootData(float deltaTime, const sf::Vector2f &playerPosition)
{
    if (shouldShoot(deltaTime))
    {
        auto position = sprite.getPosition();
        auto direction = GameUtils::normaliseVector(playerPosition - sprite.getPosition());
        return ProjectileData(
            position,
            direction * Constants::PROJECTILE_SPEED_BULLET_ALIEN,
            Constants::PROJECTILE_DAMAGE_BULLET_ALIEN);
    }

    return std::nullopt;
}

// Privates
bool ShooterEnemy::shouldShoot(float deltaTime)
{
    static float shootCooldown = 1.0f;
    static float timeSinceLastShot = 0.0f;
    timeSinceLastShot += deltaTime;

    if (timeSinceLastShot >= shootCooldown)
    {
        timeSinceLastShot -= shootCooldown;
        return true;
    }

    return false;
}