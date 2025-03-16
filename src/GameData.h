#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <SFML/System/Vector2.hpp>

#include "Projectile.h"

class GameData
{
private:
    std::vector<Projectile> projectiles;

public:
    GameData();
    ~GameData();

    std::vector<Projectile> &getProjectiles();
    void addProjectile(const Projectile &projectile);
    void removeProjectile(size_t index);
    void reset();
};

#endif // GAMEDATA_H
