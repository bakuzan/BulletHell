#include "BasicEnemy.h"
#include "constants/Constants.h"
#include "utils/GameUtils.h"

BasicEnemy::BasicEnemy(const sf::Texture &texture, sf::IntRect textureRect,
                       sf::Vector2f spawnPosition, EnemyStats stats)
    : Enemy(EnemyType::BASIC,
            texture, textureRect,
            spawnPosition,
            stats)
{
    sprite.setOrigin(Constants::SPRITE_WIDTH_BASIC / 2.0f, Constants::SPRITE_HEIGHT_BASIC / 2.0f);
    sprite.setScale(0.4, 0.4);
}

BasicEnemy::~BasicEnemy()
{
    // Destructor
}

// Publics
void BasicEnemy::update(float deltaTime, sf::RenderWindow &window, const sf::Vector2f &playerPosition)
{
    sf::Vector2f direction = GameUtils::normaliseVector(playerPosition - sprite.getPosition());
    sprite.move(direction * stats.speed * deltaTime);

    GameUtils::rotateTowards(
        sprite,
        sprite.getPosition(),
        playerPosition,
        90.0f);
}