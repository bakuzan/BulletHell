#ifndef BOSSENEMY_H
#define BOSSENEMY_H

#include <optional>

#include "RangedEnemy.h"
#include "constants/WeaponType.h"
#include "data/ProjectileData.h"
#include "data/EnemyStats.h"
#include "ui/HealthBar.h"

class BossEnemy : public RangedEnemy
{
private:
    HealthBar healthBar;
    std::optional<ProjectileData> pendingShootData;
    float laserCountdown;

    float laserTelegraphTimer;
    sf::Color currentSpriteColour;

    bool isCharging;
    bool isTelegraphingCharge;
    float chargeCooldownTimer;
    float baseChargeCooldown;
    float chargeTelegraphTimer;
    float chargeSpeedMultiplier;
    sf::Vector2f chargeTargetPosition;

private:
    float calculateDistanceToPlayerMagnitude(const sf::Vector2f &playerPosition);
    void updateHealthBarPlacement(sf::RenderWindow &window);

    bool shouldShoot(float deltaTime, const sf::Vector2f &playerPosition);
    WeaponType getWeightedWeaponType(float healthPercentage);

    std::optional<ProjectileData> prepareShootData(
        float deltaTime,
        const sf::Vector2f &playerPosition);

public:
    static constexpr float rotationOffset = 90.0f;

public:
    BossEnemy(const sf::Texture &borderTexture, const sf::Texture &fillingTexture,
              const sf::Texture &texture, sf::IntRect textureRect,
              sf::Vector2f spawnPosition, EnemyStats enemyStats);
    ~BossEnemy() override;

    void update(float deltaTime,
                sf::RenderWindow &window,
                const sf::Vector2f &playerPosition) override;

    std::optional<ProjectileData> getShootData(float deltaTime, const sf::Vector2f &playerPosition) override;

    void updateHealth(float adjustment) override;
    HealthBar &getHealthBar();
};

#endif // BOSSENEMY_H
