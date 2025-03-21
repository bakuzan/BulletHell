#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>

#include "GameData.h"
#include "State.h"
#include "StateManager.h"

class GameState : public State
{
private:
    GameData &gameData;
    StateManager &stateManager;
    sf::RenderWindow &window;

    sf::Texture backgroundTexture;
    sf::Texture spaceshipsTexture;
    sf::Texture projectileTexture;
    sf::RectangleShape background;
    sf::Sprite player;
    sf::View view;

    bool shootProjectile;

public:
    GameState(GameData &data, StateManager &manager, sf::RenderWindow &window);
    ~GameState();

    void handleEvent(const sf::Event &event) override;
    void update(sf::Time deltaTime, sf::RenderWindow &window) override;
    void render(sf::RenderWindow &window) override;
};

#endif // GAMESTATE_H
