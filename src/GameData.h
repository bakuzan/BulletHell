#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <SFML/System/Vector2.hpp>

class GameData
{
private:
    sf::Vector2f playerPosition;

public:
    GameData();
    ~GameData();

    void setPlayerPosition(sf::Vector2f playerPosition);
    void reset();
};

#endif // GAMEDATA_H
