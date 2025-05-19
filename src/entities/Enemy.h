#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

#include "constants/EnemyType.h"
#include "data/EnemyStats.h"

class Enemy
{
protected:
    EnemyType type;
    sf::Sprite sprite;

    EnemyStats initialStats;
    EnemyStats stats;

public:
    Enemy(EnemyType t,
          const sf::Texture &texture, sf::IntRect textureRect,
          sf::Vector2f spawnPosition,
          EnemyStats enemyStats);
    virtual ~Enemy();

    virtual void update(float deltaTime,
                        sf::RenderWindow &window, const sf::Vector2f &playerPosition) = 0;

    virtual void render(sf::RenderWindow &window) const;

    const EnemyType getType() const;
    const float getHealth() const;
    const sf::Sprite &getSprite() const;
    const int getPointsValue() const;
    void setPosition(sf::Vector2f position);
    virtual void updateHealth(float adjustment);
};

#endif // ENEMY_H
