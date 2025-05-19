#ifndef SHOOTERENEMY_H
#define SHOOTERENEMY_H

#include <SFML/Graphics.hpp>
#include <optional>

#include "RangedEnemy.h"
#include "data/ProjectileData.h"
#include "data/EnemyStats.h"

class ShooterEnemy : public RangedEnemy
{
private:
    bool shouldShoot(float deltaTime, const sf::Vector2f &playerPosition);
    float calculateDistanceToPlayerMagnitude(const sf::Vector2f &playerPosition);

public:
    static constexpr float rotationOffset = 270.0f;

public:
    ShooterEnemy(const sf::Texture &texture, sf::IntRect textureRect,
                 sf::Vector2f spawnPosition, EnemyStats enemyStats);
    ~ShooterEnemy() override;

    void update(float deltaTime, sf::RenderWindow &window, const sf::Vector2f &playerPosition) override;
    std::optional<ProjectileData> getShootData(float deltaTime, const sf::Vector2f &playerPosition) override;
};

#endif // SHOOTERENEMY_H
