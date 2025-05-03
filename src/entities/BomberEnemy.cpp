#include <cmath>

#include "BomberEnemy.h"
#include "constants/Constants.h"
#include "constants/ProjectileType.h"
#include "utils/GameUtils.h"
#include "components/WeaponAttributesManager.h"

BomberEnemy::BomberEnemy(
    const sf::Texture &texture, sf::IntRect textureRect,
    sf::Vector2f spawnPosition, float movementSpeed)
    : Enemy(EnemyType::BOMBER,
            texture, textureRect,
            spawnPosition,
            movementSpeed, Constants::ENEMY_POINTS_BOMBER, Constants::ENEMY_HEALTH_BOMBER)
{
    sprite.setOrigin(Constants::SPRITE_WIDTH_BOMBER / 2.0f, Constants::SPRITE_HEIGHT_BOMBER / 2.0f);
    sprite.setScale(0.66, 0.66);
}

BomberEnemy::~BomberEnemy()
{
    // Destructor
}

// Publics

void BomberEnemy::update(float deltaTime, sf::RenderWindow &window, const sf::Vector2f &playerPosition)
{
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    float magnitude = calculateDistanceToPlayerMagnitude(playerPosition);

    if (magnitude > 0)
    {
        direction /= magnitude; // Normalize the vector
    }

    sprite.move(direction * speed * deltaTime);

    GameUtils::rotateTowards(
        sprite,
        sprite.getPosition(),
        playerPosition,
        rotationOffset);
}

bool BomberEnemy::shouldDetonate(const sf::Vector2f &playerPosition)
{
    float magnitude = calculateDistanceToPlayerMagnitude(playerPosition);
    return magnitude < Constants::ENEMY_BOMBER_DISTANCE;
}

// Privates

float BomberEnemy::calculateDistanceToPlayerMagnitude(const sf::Vector2f &playerPosition)
{
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    return std::sqrt(direction.x * direction.x + direction.y * direction.y);
}
