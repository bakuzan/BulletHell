#include <cmath>

#include "BasicEnemy.h"
#include "Constants.h"
#include "utils/GameUtils.h"

BasicEnemy::BasicEnemy(const sf::Texture &texture, sf::IntRect textureRect, sf::Vector2f spawnPosition, float movementSpeed)
    : Enemy(EnemyType::BASIC, texture, textureRect, spawnPosition, movementSpeed, Constants::ENEMY_POINTS_BASIC)
{
    sprite.setScale(0.2, 0.2);
    sprite.setOrigin(Constants::SPRITE_WIDTH_BASIC / 2.0f, Constants::SPRITE_HEIGHT_BASIC / 2.0f);
}

BasicEnemy::~BasicEnemy()
{
    // Destructor
}

// Publics
void BasicEnemy::update(float deltaTime, const sf::Vector2f &playerPosition)
{
    sf::Vector2f direction = GameUtils::normaliseVector(playerPosition - sprite.getPosition());
    sprite.move(direction * speed * deltaTime);

    GameUtils::rotateTowards(
        sprite,
        sprite.getPosition(),
        playerPosition,
        90.0f);
}