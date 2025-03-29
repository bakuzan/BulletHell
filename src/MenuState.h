#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameData.h"
#include "State.h"
#include "StateManager.h"

class MenuState : public State
{
private:
    sf::Text gameTitle;
    sf::Text newGameButton;
    sf::Text quitButton;
    GameData &gameData;
    StateManager &stateManager;
    sf::RenderWindow &window;

    sf::RectangleShape background;
    int selectedButtonIndex = 0;

private:
    void updateMenuItemPositions();
    void onNewGameClick();

public:
    MenuState(GameData &data, StateManager &manager, sf::RenderWindow &window);
    ~MenuState();

    void handleEvent(const sf::Event &event) override;
    void update(sf::Time deltaTime, sf::RenderWindow &window) override;
    void render(sf::RenderWindow &window) override;
};

#endif // MENUSTATE_H
