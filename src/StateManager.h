#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "State.h"
#include <memory>
#include <stack>

class StateManager
{
private:
    std::vector<std::unique_ptr<State>> states;

public:
    StateManager();
    ~StateManager();

    void handleEvent(const sf::Event &event);
    void update(sf::Time deltaTime, sf::RenderWindow &window);
    void render(sf::RenderWindow &window);

    void pushState(std::unique_ptr<State> state);
    void popState();
    void changeState(std::unique_ptr<State> state);

    State *getCurrentState();
};

#endif // STATEMANAGER_H
