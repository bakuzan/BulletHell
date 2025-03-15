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

void GameData::setPlayerPosition(sf::Vector2f playerPosition)
{
    this->playerPosition = playerPosition;
}

void GameData::reset()
{
}