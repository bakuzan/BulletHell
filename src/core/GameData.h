#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <SFML/System/Vector2.hpp>

#include <memory>

#include "entities/Enemy.h"
#include "entities/Player.h"
#include "entities/Projectile.h"
#include "entities/UpgradeBox.h"
#include "components/EnemyTextureManager.h"
#include "components/ProjectileTextureManager.h"
#include "components/UpgradeBoxTextureManager.h"
#include "TextureManager.h"

class GameData
{
private:
    std::vector<std::unique_ptr<Projectile>> projectiles;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<UpgradeBox> upgradeBoxes;

    std::unique_ptr<Player> player;
    int playerScore;

public:
    sf::Font gameFont;

    TextureManager textureManager;

public:
    GameData();
    ~GameData();

    std::vector<std::unique_ptr<Projectile>> &getProjectiles();
    std::vector<std::unique_ptr<Enemy>> &getEnemies();
    std::vector<UpgradeBox> &getUpgradeBoxes();
    std::unique_ptr<Player> &getPlayer();

    const int getScore() const;
    void updateScore(int adjustment);

    void reset();
};

#endif // GAMEDATA_H
