#ifndef BOMBERENEMY_H
#define BOMBERENEMY_H

#include <SFML/Graphics.hpp>

#include "Enemy.h"
#include "data/EnemyStats.h"

class BomberEnemy : public Enemy
{
private:
    float calculateDistanceToPlayerMagnitude(const sf::Vector2f &playerPosition);

public:
    static constexpr float rotationOffset = 90.0f;

public:
    BomberEnemy(const sf::Texture &texture, sf::IntRect textureRect,
                sf::Vector2f spawnPosition, EnemyStats enemyStats);
    ~BomberEnemy() override;

    void update(float deltaTime, sf::RenderWindow &window, const sf::Vector2f &playerPosition) override;
    bool shouldDetonate(const sf::Vector2f &playerPosition);
};

#endif // BOMBERENEMY_H
