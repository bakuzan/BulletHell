#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

#include "EnemyType.h"

class Enemy
{
protected:
    EnemyType type;
    sf::Sprite sprite;
    int pointsValue;
    float speed;

public:
    Enemy(EnemyType t,
          const sf::Texture &texture, sf::IntRect textureRect,
          sf::Vector2f spawnPosition, float movementSpeed, int pointsValue);

    virtual void update(float deltaTime, const sf::Vector2f &playerPosition) = 0;

    virtual ~Enemy();

    void render(sf::RenderWindow &window) const;

    const EnemyType getType() const;
    const sf::Sprite &getSprite() const;
    const int getPointsValue() const;
    void setPosition(sf::Vector2f position);
};

#endif // ENEMY_H
