#include "SpeedyEnemy.h"

#include <cmath>

#include "SpeedyEnemy.h"
#include "constants/Constants.h"
#include "utils/GameUtils.h"

SpeedyEnemy::SpeedyEnemy(const sf::Texture &texture, sf::IntRect textureRect,
                         sf::Vector2f spawnPosition, EnemyStats enemyStats)
    : Enemy(EnemyType::SPEEDY,
            texture, textureRect,
            spawnPosition,
            enemyStats)
{
    sprite.setOrigin(Constants::SPRITE_WIDTH_SPEEDY / 2.0f, Constants::SPRITE_HEIGHT_SPEEDY / 2.0f);
    sprite.setScale(0.3, 0.3);
}

SpeedyEnemy::~SpeedyEnemy()
{
    // Destructor
}

// Publics
void SpeedyEnemy::update(float deltaTime, sf::RenderWindow &window, const sf::Vector2f &playerPosition)
{
    sf::Vector2f direction = GameUtils::normaliseVector(playerPosition - sprite.getPosition());
    sprite.move(direction * stats.speed * deltaTime);

    GameUtils::rotateTowards(
        sprite,
        sprite.getPosition(),
        playerPosition,
        270.0f);
}
