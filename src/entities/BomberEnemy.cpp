#include <cmath>

#include "BomberEnemy.h"
#include "constants/Constants.h"
#include "constants/ProjectileType.h"
#include "utils/GameUtils.h"
#include "components/WeaponAttributesManager.h"

BomberEnemy::BomberEnemy(
    const sf::Texture &texture, sf::IntRect textureRect,
    sf::Vector2f spawnPosition, EnemyStats enemyStats)
    : Enemy(EnemyType::BOMBER,
            texture, textureRect,
            spawnPosition,
            enemyStats)
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

    sprite.move(direction * stats.speed * deltaTime);

    GameUtils::rotateTowards(
        sprite,
        sprite.getPosition(),
        playerPosition,
        rotationOffset);
}

bool BomberEnemy::shouldDetonate(const sf::Vector2f &playerPosition)
{
    float magnitude = calculateDistanceToPlayerMagnitude(playerPosition);
    return magnitude < stats.activeDistance;
}

// Privates

float BomberEnemy::calculateDistanceToPlayerMagnitude(const sf::Vector2f &playerPosition)
{
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    return std::sqrt(direction.x * direction.x + direction.y * direction.y);
}
