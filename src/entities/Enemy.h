#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

#include "constants/EnemyType.h"

class Enemy
{
protected:
    EnemyType type;
    sf::Sprite sprite;

    int pointsValue;
    float health;
    float speed;

public:
    Enemy(EnemyType t,
          const sf::Texture &texture, sf::IntRect textureRect,
          sf::Vector2f spawnPosition,
          float movementSpeed, int pointsValue, float startingHealth);

    virtual void update(float deltaTime,
                        sf::RenderWindow &window, const sf::Vector2f &playerPosition) = 0;

    virtual ~Enemy();

    void render(sf::RenderWindow &window) const;

    const EnemyType getType() const;
    const float getHealth() const;
    const sf::Sprite &getSprite() const;
    const int getPointsValue() const;
    void setPosition(sf::Vector2f position);
    void updateHealth(float adjustment);
};

#endif // ENEMY_H
