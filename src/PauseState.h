#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include <SFML/Graphics.hpp>

#include "GameData.h"
#include "State.h"
#include "StateManager.h"

class PauseState : public State
{
private:
    GameData &gameData;
    StateManager &stateManager;
    sf::RenderWindow &window;

    sf::Text pauseText;

public:
    PauseState(GameData &data, StateManager &manager, sf::RenderWindow &window);
    ~PauseState();

    void handleEvent(const sf::Event &event) override;
    void update(sf::Time deltaTime, sf::RenderWindow &window) override;
    void render(sf::RenderWindow &window) override;
};

#endif // PAUSESTATE_H
