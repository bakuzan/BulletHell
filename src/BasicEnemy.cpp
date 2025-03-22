#include <cmath>

#include "BasicEnemy.h"

BasicEnemy::BasicEnemy(const sf::Texture &texture, sf::IntRect textureRect, sf::Vector2f spawnPosition, float movementSpeed)
    : Enemy(EnemyType::BASIC, texture, textureRect, spawnPosition, movementSpeed)
{
    sprite.setScale(0.2, 0.2);
}

BasicEnemy::~BasicEnemy()
{
    // Destructor
}

// Publics
void BasicEnemy::update(float deltaTime, sf::Vector2f playerPosition)
{
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (magnitude > 0)
    {
        direction /= magnitude; // Normalize the vector
    }

    sprite.move(direction * speed * deltaTime);
}