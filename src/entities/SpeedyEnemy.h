#ifndef SPEEDYENEMY_H
#define SPEEDYENEMY_H

#include "Enemy.h"
#include "data/EnemyStats.h"

class SpeedyEnemy : public Enemy
{
public:
    SpeedyEnemy(const sf::Texture &texture, sf::IntRect textureRect,
                sf::Vector2f spawnPosition, EnemyStats enemyStats);
    ~SpeedyEnemy() override;

    void update(float deltaTime, sf::RenderWindow &window, const sf::Vector2f &playerPosition) override;
};

#endif // SPEEDYENEMY_H
