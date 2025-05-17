#ifndef UPGRADEBOXSPAWNMANAGER_H
#define UPGRADEBOXSPAWNMANAGER_H

#include "constants/Direction.h"
#include "constants/UpgradeBoxType.h"
#include "entities/UpgradeBox.h"
#include "TextureRectManager.h"

class UpgradeBoxSpawnManager
{
private:
    const TextureRectManager &upgradeBoxRectManager;

    float healthBoxTimer;
    float weaponBoxTimer;

private:
    UpgradeBox spawnBox(UpgradeBoxType boxType,
                        const sf::Texture &texture,
                        const Direction &playerDirection,
                        const sf::View &view);

    sf::Vector2f calculateSpawnPosition(
        const Direction &playerDirection,
        const sf::View &view);

    UpgradeBoxType randomWeaponType();

    void resetHealthBoxTimer();
    void resetWeaponBoxTimer();

public:
    UpgradeBoxSpawnManager(const TextureRectManager &textureRectManager);
    ~UpgradeBoxSpawnManager();

    void spawnUpgradeBoxes(float deltaTime,
                           std::vector<UpgradeBox> &upgradeBoxes,
                           const sf::Texture &upgradeBoxesTexture,
                           const Direction &playerDirection,
                           const sf::View &view);
};

#endif // UPGRADEBOXSPAWNMANAGER_H
