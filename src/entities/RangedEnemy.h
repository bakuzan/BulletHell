#ifndef RANGEDENEMY_H
#define RANGEDENEMY_H

#include <SFML/Graphics.hpp>
#include <optional>

#include "Enemy.h"
#include "data/ProjectileData.h"
#include "data/EnemyStats.h"

class RangedEnemy : public Enemy
{
public:
    RangedEnemy(EnemyType t,
                const sf::Texture &texture, sf::IntRect textureRect,
                sf::Vector2f spawnPosition,
                EnemyStats enemyStats);
    ~RangedEnemy() override;

    virtual std::optional<ProjectileData> getShootData(float deltaTime, const sf::Vector2f &playerPosition) = 0;
};

#endif // RANGEDENEMY_H
