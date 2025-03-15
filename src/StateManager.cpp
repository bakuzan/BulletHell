#include "StateManager.h"

StateManager::StateManager()
{
    // Constructor
}

StateManager::~StateManager()
{
    // Destructor
}

void StateManager::pushState(std::unique_ptr<State> state)
{
    states.push(std::move(state));
}

void StateManager::popState()
{
    if (!states.empty())
    {
        states.pop();
    }
}

void StateManager::changeState(std::unique_ptr<State> state)
{
    if (!states.empty())
    {
        states.pop();
    }

    states.push(std::move(state));
}

State *StateManager::getCurrentState()
{
    return states.empty() ? nullptr : states.top().get();
}