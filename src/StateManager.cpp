#include "StateManager.h"

StateManager::StateManager()
{
    // Constructor
}

StateManager::~StateManager()
{
    // Destructor
}

// Publics

void StateManager::handleEvent(const sf::Event &event)
{
    if (!states.empty())
    {
        states.back()->handleEvent(event);
    }
}

void StateManager::update(sf::Time deltaTime, sf::RenderWindow &window)
{
    if (!states.empty())
    {
        states.back()->update(deltaTime, window);
    }
}

void StateManager::render(sf::RenderWindow &window)
{
    for (auto &state : states)
    {
        state->render(window);
    }
}

void StateManager::pushState(std::unique_ptr<State> state)
{
    states.push_back(std::move(state));
}

void StateManager::popState()
{
    if (!states.empty())
    {
        states.pop_back();
    }
}

void StateManager::changeState(std::unique_ptr<State> state)
{
    if (!states.empty())
    {
        states.pop_back();
    }

    states.push_back(std::move(state));
}

void StateManager::replaceStates(std::unique_ptr<State> state)
{
    states.clear();
    states.push_back(std::move(state));
}
