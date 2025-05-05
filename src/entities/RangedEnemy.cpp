#include "RangedEnemy.h"

RangedEnemy::RangedEnemy(EnemyType t,
                         const sf::Texture &texture, sf::IntRect textureRect,
                         sf::Vector2f spawnPosition,
                         float movementSpeed, int pointsValue, float startingHealth)
    : Enemy(t,
            texture, textureRect,
            spawnPosition,
            movementSpeed, pointsValue, startingHealth)
{
    // Constructor
}

RangedEnemy::~RangedEnemy()
{
    // Destructor
}
