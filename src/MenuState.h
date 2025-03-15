#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameData.h"
#include "State.h"

class MenuState : public State
{
private:
    sf::RectangleShape background;
    GameData &gameData;

public:
    MenuState(GameData &data);
    ~MenuState();

    void handleEvent(const sf::Event &event) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow &window) override;
};

#endif // MENUSTATE_H
