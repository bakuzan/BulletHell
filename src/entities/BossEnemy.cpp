#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "BossEnemy.h"
#include "constants/Constants.h"
#include "constants/ProjectileType.h"
#include "utils/GameUtils.h"
#include "components/WeaponAttributesManager.h"

BossEnemy::BossEnemy(
    const sf::Texture &borderTexture, const sf::Texture &fillingTexture,
    const sf::Texture &texture, sf::IntRect textureRect,
    sf::Vector2f spawnPosition, EnemyStats enemyStats)
    : RangedEnemy(EnemyType::BOSS,
                  texture, textureRect,
                  spawnPosition,
                  enemyStats),
      healthBar(borderTexture, fillingTexture,
                enemyStats.health, enemyStats.health,
                "Boss"),
      laserCountdown(0.0f),
      laserTelegraphTimer(0.0f),
      currentSpriteColour(sf::Color::White),
      isCharging(false), isTelegraphingCharge(false),
      chargeCooldownTimer(0.0f), baseChargeCooldown(8.0f),
      chargeTelegraphTimer(0.25f), chargeSpeedMultiplier(10.0f)
{
    sprite.setOrigin(Constants::SPRITE_WIDTH_BOSS / 2.0f, Constants::SPRITE_HEIGHT_BOSS / 2.0f);
}

BossEnemy::~BossEnemy()
{
    // Destructor
}

// Publics

void BossEnemy::update(float deltaTime,
                       sf::RenderWindow &window,
                       const sf::Vector2f &playerPosition)
{
    // Do standard Boss turing/moving and health adjustments
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    float magnitude = calculateDistanceToPlayerMagnitude(playerPosition);

    if (magnitude > 0)
    {
        direction /= magnitude; // Normalize the vector
    }

    if (chargeCooldownTimer > 0)
    {
        chargeCooldownTimer -= deltaTime;
    }

    // Decide to charge?
    if (!isCharging &&
        !isTelegraphingCharge &&
        chargeCooldownTimer <= 0 &&
        !pendingShootData.has_value())
    {
        if (rand() % 100 == 0)
        {
            isTelegraphingCharge = true;
            chargeTargetPosition = playerPosition;
            chargeTelegraphTimer = 0.25f;
            currentSpriteColour = sf::Color::Red;
        }
    }

    // Telegraph phase
    if (isTelegraphingCharge)
    {
        chargeTelegraphTimer -= deltaTime;

        if (chargeTelegraphTimer <= 0)
        {
            isTelegraphingCharge = false;
            isCharging = true;
            currentSpriteColour = sf::Color::White;
        }
    }

    if (isCharging)
    {
        sf::Vector2f chargeDirection = chargeTargetPosition - sprite.getPosition();
        float chargeMagnitude = std::sqrt(chargeDirection.x * chargeDirection.x + chargeDirection.y * chargeDirection.y);

        if (chargeMagnitude > 0)
        {
            chargeDirection /= chargeMagnitude;
        }

        sprite.move(chargeDirection * stats.speed * chargeSpeedMultiplier * deltaTime);

        // Stop charging once close enough to the target position
        if (chargeMagnitude < 10.0f)
        {
            isCharging = false;
            chargeCooldownTimer = std::max(3.0f, baseChargeCooldown * (stats.health / initialStats.health));
        }
    }
    else
    {
        if (magnitude > stats.activeDistance)
        {
            // Player is running! Force the boss closer!
            sf::Vector2f clampedPosition = playerPosition - direction * stats.activeDistance;
            sprite.setPosition(clampedPosition);
        }
        else
        {
            sprite.move(direction * stats.speed * deltaTime);
        }
    }

    if (!pendingShootData.has_value() &&
        !isCharging &&
        !isTelegraphingCharge)
    {
        // Only turn if Boss isn't "charging" laser or charging at the player
        GameUtils::rotateTowards(
            sprite,
            sprite.getPosition(),
            playerPosition,
            rotationOffset);
    }

    updateHealthBarPlacement(window);

    // Handle Boss shooting
    if (!pendingShootData.has_value() &&
        !isCharging &&
        !isTelegraphingCharge)
    {
        currentSpriteColour = sf::Color::White;

        pendingShootData = prepareShootData(deltaTime, playerPosition);
        if (pendingShootData.has_value())
        {
            if (pendingShootData->type == ProjectileType::ALIEN_LASER)
            {
                laserCountdown = 0.3f;
                currentSpriteColour = sf::Color::Green;
            }
        }
    }
    else if (pendingShootData.has_value() &&
             pendingShootData->type == ProjectileType::ALIEN_LASER)
    {
        laserCountdown -= deltaTime;
        laserTelegraphTimer += deltaTime;

        if (laserTelegraphTimer >= 0.1f)
        {
            laserTelegraphTimer = 0.0f;
            currentSpriteColour = currentSpriteColour == sf::Color::White
                                      ? sf::Color::Green
                                      : sf::Color::White;
        }
    }

    sprite.setColor(currentSpriteColour);
}

std::optional<ProjectileData> BossEnemy::getShootData(
    float deltaTime,
    const sf::Vector2f &playerPosition)
{
    if (pendingShootData.has_value())
    {
        if (pendingShootData->type != ProjectileType::ALIEN_LASER ||
            laserCountdown <= 0.0f)
        {
            auto shootData = pendingShootData;
            pendingShootData.reset();
            return shootData;
        }
    }

    return std::nullopt;
}

void BossEnemy::updateHealth(float adjustment)
{
    Enemy::updateHealth(adjustment);
    healthBar.setHealth(getHealth());
}

HealthBar &BossEnemy::getHealthBar()
{
    return healthBar;
}

// Privates

float BossEnemy::calculateDistanceToPlayerMagnitude(const sf::Vector2f &playerPosition)
{
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    return std::sqrt(direction.x * direction.x + direction.y * direction.y);
}

void BossEnemy::updateHealthBarPlacement(sf::RenderWindow &window)
{
    sf::View view = window.getDefaultView();
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f viewCenter = view.getCenter();

    // // Resize health bar to match width (minus margins)
    float newWidth = viewSize.x - 20.0f;
    healthBar.scaleBasedOnTargetWidth(newWidth);

    // Calculate bottom-left starting position
    float xPos = viewCenter.x - viewSize.x / 2.0f + 10.0f;
    float yPos = viewCenter.y + viewSize.y / 2.0f - 60.0f;
    healthBar.setPosition(xPos, yPos);
}

bool BossEnemy::shouldShoot(float deltaTime,
                            const sf::Vector2f &playerPosition)
{
    static float timeSinceLastShot = 0.0f;

    float healthPercentage = stats.health / initialStats.health;
    float shootCooldown = stats.fireRateCooldown * (0.2f + 0.8f * healthPercentage);

    timeSinceLastShot += deltaTime;

    if (timeSinceLastShot >= shootCooldown)
    {
        timeSinceLastShot -= shootCooldown;
        return true;
    }

    return false;
}

WeaponType BossEnemy::getWeightedWeaponType(float healthPercentage)
{
    std::vector<WeaponType> weaponTypes = {
        WeaponType::ALIEN_BASIC,
        WeaponType::ALIEN_SEEKER,
        WeaponType::ALIEN_LASER};

    // Adjust probabilities based on health percentage
    std::vector<int> weights = {
        static_cast<int>(100 * (healthPercentage)),
        static_cast<int>(100 * (1.0 - healthPercentage)),
        static_cast<int>(100 * (1.0 - healthPercentage) * 2)};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist(weights.begin(), weights.end());

    return weaponTypes[dist(gen)];
}

std::optional<ProjectileData> BossEnemy::prepareShootData(
    float deltaTime,
    const sf::Vector2f &playerPosition)
{
    if (shouldShoot(deltaTime, playerPosition))
    {
        float healthPercentage = stats.health / initialStats.health;
        WeaponType selectedType = getWeightedWeaponType(healthPercentage);
        WeaponAttributes weaponAttrs = WeaponAttributesManager::getInstance()
                                           .getAttributes(selectedType);

        SpawnData projectileSpawnData =
            GameUtils::getSpawnDataForProjectileFromEntity(
                sprite,
                weaponAttrs,
                rotationOffset);

        return ProjectileData::CreateRegular(
            weaponAttrs,
            projectileSpawnData);
    }

    return std::nullopt;
}