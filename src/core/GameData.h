#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <SFML/System/Vector2.hpp>

#include <memory>

#include "entities/Enemy.h"
#include "entities/Projectile.h"
#include "entities/UpgradeBox.h"
#include "components/EnemyTextureManager.h"
#include "components/ProjectileTextureManager.h"
#include "components/UpgradeBoxTextureManager.h"
#include "TextureManager.h"

class GameData
{
private:
    std::vector<Projectile> projectiles;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<UpgradeBox> upgradeBoxes;

    float playerHealth;
    int playerScore;

public:
    sf::Font gameFont;

    TextureManager textureManager;
    ProjectileTextureManager projectileTextureManager;
    UpgradeBoxTextureManager upgradeBoxTextureManager;

public:
    GameData();
    ~GameData();

    std::vector<Projectile> &getProjectiles();
    void addProjectile(const Projectile &projectile);
    void removeProjectile(size_t index);

    std::vector<std::unique_ptr<Enemy>> &getEnemies();
    void addEnemy(std::unique_ptr<Enemy> enemy);
    void removeEnemy(size_t index);

    std::vector<UpgradeBox> &getUpgradeBoxes();
    void addUpgradeBox(const UpgradeBox &upgrade);
    void removeUpgradeBox(size_t index);

    const float getPlayerHealth() const;
    void updatePlayerHealth(float adjustment);
    const int getScore() const;
    void updateScore(int adjustment);

    void reset();
};

#endif // GAMEDATA_H
