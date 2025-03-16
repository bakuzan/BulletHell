#include "GameData.h"

GameData::GameData()
{
    // Constructor
}

GameData::~GameData()
{
    // Destructor
}

// Publics

std::vector<Projectile> &GameData::getProjectiles()
{
    return projectiles;
}

void GameData::addProjectile(const Projectile &projectile)
{
    projectiles.push_back(projectile);
}

void GameData::removeProjectile(size_t index)
{
    projectiles.erase(projectiles.begin() + index);
}

void GameData::reset()
{
}