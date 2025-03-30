#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include <SFML/Graphics.hpp>

#include "Button.h"
#include "GameData.h"
#include "State.h"
#include "StateManager.h"

class PauseState : public State
{
private:
    GameData &gameData;
    StateManager &stateManager;
    sf::RenderWindow &window;

    sf::RectangleShape background;
    sf::Text pauseText;
    float buttonSpacing;
    std::vector<Button> buttons;
    int selectedButtonIndex = 0;

private:
    void updateMenuItemPositions();

public:
    PauseState(GameData &data, StateManager &manager, sf::RenderWindow &window);
    ~PauseState();

    void handleEvent(const sf::Event &event) override;
    void update(sf::Time deltaTime, sf::RenderWindow &window) override;
    void render(sf::RenderWindow &window) override;
};

#endif // PAUSESTATE_H
