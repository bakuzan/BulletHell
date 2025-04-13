#ifndef UPGRADEBOXSPAWNMANAGER_H
#define UPGRADEBOXSPAWNMANAGER_H

#include "UpgradeBoxType.h"
#include "entities/UpgradeBox.h"
#include "UpgradeBoxTextureManager.h"

class UpgradeBoxSpawnManager
{
private:
    UpgradeBoxTextureManager upgradeBoxTextureManager;

    float healthBoxTimer;
    float weaponBoxTimer;

private:
    UpgradeBox spawnBox(UpgradeBoxType boxType,
                        const sf::Texture &texture,
                        const sf::Vector2f &playerPosition,
                        const sf::Vector2f &playerDirection,
                        const sf::View &view);

    sf::Vector2f calculateSpawnPosition(const sf::Vector2f &playerPosition,
                                        const sf::Vector2f &playerDirection,
                                        const sf::View &view);

    UpgradeBoxType randomWeaponType();

    void resetHealthBoxTimer();
    void resetWeaponBoxTimer();

public:
    UpgradeBoxSpawnManager();
    ~UpgradeBoxSpawnManager();

    void spawnUpgradeBoxes(float deltaTime,
                           std::vector<UpgradeBox> &upgradeBoxes,
                           const sf::Texture &upgradeBoxesTexture,
                           const sf::Vector2f &playerPosition,
                           const sf::Vector2f &playerDirection,
                           const sf::View &view);
};

#endif // UPGRADEBOXSPAWNMANAGER_H
