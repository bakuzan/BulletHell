#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <memory>
#include <stack>

#include "State.h"

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
    void replaceStates(std::unique_ptr<State> state);
};

#endif // STATEMANAGER_H
