#include "GameData.h"

GameData::GameData()
{
    textureManager.loadTexture(TextureId::BACKGROUND, "resources/background.png", true);
    textureManager.loadTexture(TextureId::SPACESHIPS, "resources/spaceships_brighter.png");
    textureManager.loadTexture(TextureId::PROJECTILES, "resources/projectiles.png");
    textureManager.loadTexture(TextureId::HEALTHBAR_BORDER, "resources/healthbar_border.png");
    textureManager.loadTexture(TextureId::HEALTHBAR_FILLING, "resources/healthbar_filling.png");

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
    playerHealth = std::max(0.0f, playerHealth + adjustment); // Clamp at 0
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