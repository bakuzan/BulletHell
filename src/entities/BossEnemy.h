#ifndef BOSSENEMY_H
#define BOSSENEMY_H

#include "Enemy.h"
#include "ui/HealthBar.h"

class BossEnemy : public Enemy
{
private:
    HealthBar healthBar;

private:
    float calculateDistanceToPlayerMagnitude(const sf::Vector2f &playerPosition);
    void updateHealthBarPlacement(sf::RenderWindow &window);

public:
    static constexpr float rotationOffset = 90.0f;

public:
    BossEnemy(const sf::Texture &borderTexture, const sf::Texture &fillingTexture,
              const sf::Texture &texture, sf::IntRect textureRect,
              sf::Vector2f spawnPosition, float movementSpeed);
    ~BossEnemy() override;

    void update(float deltaTime,
                sf::RenderWindow &window,
                const sf::Vector2f &playerPosition) override;
    void render(sf::RenderWindow &window);

    void updateHealth(float adjustment);
};

#endif // BOSSENEMY_H
