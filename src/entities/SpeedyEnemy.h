#ifndef SPEEDYENEMY_H
#define SPEEDYENEMY_H

#include "Enemy.h"

class SpeedyEnemy : public Enemy
{
public:
    SpeedyEnemy(const sf::Texture &texture, sf::IntRect textureRect,
                sf::Vector2f spawnPosition, float movementSpeed);
    ~SpeedyEnemy() override;

    void update(float deltaTime, const sf::Vector2f &playerPosition) override;
};

#endif // SPEEDYENEMY_H
