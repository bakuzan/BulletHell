#ifndef SHOOTERENEMY_H
#define SHOOTERENEMY_H

#include <optional>

#include "Enemy.h"
#include "data/ProjectileData.h"

class ShooterEnemy : public Enemy
{

private:
    bool shouldShoot(float deltaTime);

public:
    static constexpr float rotationOffset = 270.0f;

public:
    ShooterEnemy(const sf::Texture &texture, sf::IntRect textureRect, sf::Vector2f spawnPosition, float movementSpeed);
    ~ShooterEnemy() override;

    void update(float deltaTime, const sf::Vector2f &playerPosition) override;
    std::optional<ProjectileData> getShootData(float deltaTime, const sf::Vector2f &playerPosition);
};

#endif // SHOOTERENEMY_H
