#include "GameData.h"

GameData::GameData()
{
    playerHealth = 100.0f;
    playerScore = 0;
}

GameData::~GameData()
{
    // Destructor
}

// Publics

// Projectiles
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

// Enemies
std::vector<std::unique_ptr<Enemy>> &GameData::getEnemies()
{
    return enemies;
}

void GameData::addEnemy(std::unique_ptr<Enemy> enemy)
{
    enemies.push_back(std::move(enemy));
}

void GameData::removeEnemy(size_t index)
{
    enemies.erase(enemies.begin() + index);
}

// Player attributes
const float GameData::getPlayerHealth() const
{
    return playerHealth;
}

void GameData::updatePlayerHealth(float adjustment)
{
    playerScore += adjustment;
}

const int GameData::getScore() const
{
    return playerScore;
}

void GameData::updateScore(int adjustment)
{
    playerScore += adjustment;
}

// State handling
void GameData::reset()
{
    projectiles.clear();
    enemies.clear();

    playerHealth = 100.0f;
    playerScore = 0;
}