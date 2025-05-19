#ifndef BASICENEMY_H
#define BASICENEMY_H

#include "Enemy.h"
#include "data/EnemyStats.h"

class BasicEnemy : public Enemy
{
public:
    BasicEnemy(const sf::Texture &texture, sf::IntRect textureRect,
               sf::Vector2f spawnPosition, EnemyStats stats);
    ~BasicEnemy() override;

    void update(float deltaTime, sf::RenderWindow &window, const sf::Vector2f &playerPosition) override;
};

#endif // BASICENEMY_H
