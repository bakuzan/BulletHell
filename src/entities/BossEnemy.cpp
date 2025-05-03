#include <cmath>

#include "BossEnemy.h"
#include "constants/Constants.h"
#include "utils/GameUtils.h"

BossEnemy::BossEnemy(
    const sf::Texture &borderTexture, const sf::Texture &fillingTexture,
    const sf::Texture &texture, sf::IntRect textureRect,
    sf::Vector2f spawnPosition, float movementSpeed)
    : Enemy(EnemyType::BOSS,
            texture, textureRect,
            spawnPosition,
            movementSpeed, Constants::ENEMY_POINTS_BOSS, Constants::ENEMY_HEALTH_BOSS),
      healthBar(borderTexture, fillingTexture,
                Constants::ENEMY_HEALTH_BOSS, Constants::ENEMY_HEALTH_BOSS)
{
    sprite.setOrigin(Constants::SPRITE_WIDTH_BOSS / 2.0f, Constants::SPRITE_HEIGHT_BOSS / 2.0f);
}

BossEnemy::~BossEnemy()
{
    // Destructor
}

// Publics

void BossEnemy::update(float deltaTime,
                       sf::RenderWindow &window,
                       const sf::Vector2f &playerPosition)
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

    updateHealthBarPlacement(window);
}

void BossEnemy::render(sf::RenderWindow &window)
{
    Enemy::render(window);
    healthBar.render(window);
}

void BossEnemy::updateHealth(float adjustment)
{
    Enemy::updateHealth(adjustment);
    healthBar.setHealth(getHealth());
}

// Privates

float BossEnemy::calculateDistanceToPlayerMagnitude(const sf::Vector2f &playerPosition)
{
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    return std::sqrt(direction.x * direction.x + direction.y * direction.y);
}

void BossEnemy::updateHealthBarPlacement(sf::RenderWindow &window)
{
    sf::View view = window.getView();
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f viewCenter = view.getCenter();

    // // Resize health bar to match width (minus margins)
    float newWidth = viewSize.x - 40.0f;
    healthBar.scaleBasedOnTargetWidth(newWidth);

    // Calculate bottom-left starting position
    float xPos = viewCenter.x - (viewSize.x / 2.0f) + 20.0f;
    float yPos = viewCenter.y + (viewSize.y / 2.0f) - 40.0f;
    healthBar.setPosition(xPos, yPos);
}