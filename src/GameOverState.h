#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include <SFML/Graphics.hpp>

#include "Button.h"
#include "GameData.h"
#include "State.h"
#include "StateManager.h"

class GameOverState : public State
{
private:
    GameData &gameData;
    StateManager &stateManager;
    sf::RenderWindow &window;

    sf::RectangleShape background;
    sf::Text gameOverText;
    sf::Text finalScoreText;
    float buttonSpacing;
    std::vector<Button> buttons;
    int selectedButtonIndex = 0;

private:
    void updateMenuItemPositions();

public:
    GameOverState(GameData &data, StateManager &manager, sf::RenderWindow &window);
    ~GameOverState();

    void handleEvent(const sf::Event &event) override;
    void update(sf::Time deltaTime, sf::RenderWindow &window) override;
    void render(sf::RenderWindow &window) override;
};

#endif // GAMEOVERSTATE_H
