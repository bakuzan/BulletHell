#include "GameData.h"
#include "constants/Constants.h"

GameData::GameData()
{
    textureManager.loadTexture(TextureId::BACKGROUND, "resources/background_2.png", true);
    textureManager.loadTexture(TextureId::SPACESHIPS, "resources/spaceships_brighter.png");
    textureManager.loadTexture(TextureId::PROJECTILES, "resources/projectiles.png");
    textureManager.loadTexture(TextureId::HEALTHBAR_BORDER, "resources/healthbar_border.png");
    textureManager.loadTexture(TextureId::HEALTHBAR_FILLINGS, "resources/healthbar_fillings.png");
    textureManager.loadTexture(TextureId::UPGRADE_BOXES, "resources/upgrade_boxes.png");

    // Load font
    if (!gameFont.loadFromFile("resources/fonts/PressStart2P-Regular.ttf"))
    {
        throw std::runtime_error("Failed to load font");
    }

    // Init player
    player = std::make_unique<Player>(
        textureManager.getTexture(TextureId::SPACESHIPS),
        sf::IntRect(392, Constants::SPRITE_OFFSET_Y, Constants::SPRITE_WIDTH_PLAYER, Constants::SPRITE_HEIGHT_PLAYER),
        300.0f,
        100.0f);

    reset();
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

// UpgradeBoxes
std::vector<UpgradeBox> &GameData::getUpgradeBoxes()
{
    return upgradeBoxes;
}

void GameData::addUpgradeBox(const UpgradeBox &upgrade)
{
    upgradeBoxes.push_back(upgrade);
}

void GameData::removeUpgradeBox(size_t index)
{
    upgradeBoxes.erase(upgradeBoxes.begin() + index);
}

// Player
std::unique_ptr<Player> &GameData::getPlayer()
{
    return player;
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
    upgradeBoxes.clear();

    player->reset();
    playerScore = 0;
}