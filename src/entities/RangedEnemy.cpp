#include "RangedEnemy.h"

RangedEnemy::RangedEnemy(EnemyType t,
                         const sf::Texture &texture, sf::IntRect textureRect,
                         sf::Vector2f spawnPosition,
                         EnemyStats enemyStats)
    : Enemy(t,
            texture, textureRect,
            spawnPosition,
            enemyStats)
{
    // Constructor
}

RangedEnemy::~RangedEnemy()
{
    // Destructor
}
