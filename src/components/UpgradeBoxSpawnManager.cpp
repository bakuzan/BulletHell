#include "Constants.h"
#include "UpgradeBoxSpawnManager.h"

UpgradeBoxSpawnManager::UpgradeBoxSpawnManager()
{
    resetHealthBoxTimer();
    resetWeaponBoxTimer();
}

UpgradeBoxSpawnManager::~UpgradeBoxSpawnManager()
{
    // Destructor
}

// Publics

void UpgradeBoxSpawnManager::spawnUpgradeBoxes(float deltaTime,
                                               std::vector<UpgradeBox> &upgradeBoxes,
                                               const sf::Texture &upgradeBoxesTexture,
                                               const sf::Vector2f &playerPosition,
                                               const sf::Vector2f &playerDirection,
                                               const sf::View &view)
{
    weaponBoxTimer -= deltaTime;
    healthBoxTimer -= deltaTime;

    // Spawn health box
    if (healthBoxTimer <= 0.0f)
    {
        upgradeBoxes.emplace_back(
            spawnBox(UpgradeBoxType::HEALTH,
                     upgradeBoxesTexture,
                     playerPosition, playerDirection,
                     view));

        resetHealthBoxTimer();
    }

    // Spawn weapon box
    if (weaponBoxTimer <= 0.0f)
    {
        upgradeBoxes.emplace_back(
            spawnBox(randomWeaponType(),
                     upgradeBoxesTexture,
                     playerPosition, playerDirection,
                     view));

        resetWeaponBoxTimer();
    }
}

// Privates

UpgradeBox UpgradeBoxSpawnManager::spawnBox(UpgradeBoxType boxType,
                                            const sf::Texture &texture,
                                            const sf::Vector2f &playerPosition,
                                            const sf::Vector2f &playerDirection,
                                            const sf::View &view)
{
    sf::Vector2f spawnPosition = calculateSpawnPosition(playerPosition, playerDirection, view);
    float lifetime = 30.0f;

    return UpgradeBox(boxType,
                      texture, upgradeBoxTextureManager.getTextureRect(boxType),
                      spawnPosition, lifetime);
}

sf::Vector2f UpgradeBoxSpawnManager::calculateSpawnPosition(const sf::Vector2f &playerPosition,
                                                            const sf::Vector2f &playerDirection,
                                                            const sf::View &view)
{
    sf::Vector2f viewSize = view.getSize();

    // If no movement, spawn out top right
    if (playerDirection == sf::Vector2f(0.f, 0.f))
    {
        return playerPosition + sf::Vector2f(viewSize.x / 2.f + Constants::UPGRADE_BOX_SPAWN_OFFSET,
                                             viewSize.y / 2.f + Constants::UPGRADE_BOX_SPAWN_OFFSET);
    }

    sf::Vector2f offscreenOffset = sf::Vector2f(playerDirection.x * Constants::UPGRADE_BOX_SPAWN_OFFSET,
                                                playerDirection.y * Constants::UPGRADE_BOX_SPAWN_OFFSET);
    sf::Vector2f spawnOffset = playerDirection * std::max(viewSize.x, viewSize.y) / 2.f + offscreenOffset;
    return playerPosition + spawnOffset;
}

UpgradeBoxType UpgradeBoxSpawnManager::randomWeaponType()
{
    int totalTypes = static_cast<int>(UpgradeBoxType::COUNT);

    // Random excluding 0
    int randomIndex = std::rand() % (totalTypes - 1) + 1;
    return static_cast<UpgradeBoxType>(randomIndex);
}

void UpgradeBoxSpawnManager::resetHealthBoxTimer()
{
    healthBoxTimer = 6.0f;
}
void UpgradeBoxSpawnManager::resetWeaponBoxTimer()
{
    weaponBoxTimer = 3.0f;
}