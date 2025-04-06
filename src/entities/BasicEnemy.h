#ifndef BASICENEMY_H
#define BASICENEMY_H

#include "Enemy.h"

class BasicEnemy : public Enemy
{
public:
    BasicEnemy(const sf::Texture &texture, sf::IntRect textureRect, sf::Vector2f spawnPosition, float movementSpeed);
    ~BasicEnemy() override;

    void update(float deltaTime, const sf::Vector2f &playerPosition) override;
};

#endif // BASICENEMY_H
