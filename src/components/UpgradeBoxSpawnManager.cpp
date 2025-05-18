#include "constants/Constants.h"
#include "UpgradeBoxSpawnManager.h"
#include "utils/GameUtils.h"

UpgradeBoxSpawnManager::UpgradeBoxSpawnManager(const TextureRectManager &textureRectManager)
    : upgradeBoxRectManager(textureRectManager)
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
                                               const Direction &playerDirection,
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
                     playerDirection,
                     view));

        resetHealthBoxTimer();
    }

    // Spawn weapon box
    if (weaponBoxTimer <= 0.0f)
    {
        upgradeBoxes.emplace_back(
            spawnBox(randomWeaponType(),
                     upgradeBoxesTexture,
                     playerDirection,
                     view));

        resetWeaponBoxTimer();
    }
}

// Privates

UpgradeBox UpgradeBoxSpawnManager::spawnBox(UpgradeBoxType boxType,
                                            const sf::Texture &texture,
                                            const Direction &playerDirection,
                                            const sf::View &view)
{
    sf::Vector2f spawnPosition = calculateSpawnPosition(playerDirection, view);
    float lifetime = 30.0f;

    return UpgradeBox(boxType,
                      texture, upgradeBoxRectManager.getTextureRect(boxType),
                      spawnPosition, lifetime);
}

sf::Vector2f UpgradeBoxSpawnManager::calculateSpawnPosition(
    const Direction &playerDirection,
    const sf::View &view)
{
    Direction side = playerDirection;
    if (playerDirection == Direction::NONE)
    {
        int randomInt = (rand() % 4) + 1; // Choose a random side around the player
        side = static_cast<Direction>(randomInt);
    }

    return GameUtils::getRandomPositionOnSide(view, side, Constants::UPGRADE_BOX_SPAWN_OFFSET);
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
    healthBoxTimer = 48.0f;
}
void UpgradeBoxSpawnManager::resetWeaponBoxTimer()
{
    weaponBoxTimer = 30.0f;
}