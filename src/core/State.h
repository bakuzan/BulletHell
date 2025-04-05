#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>

class State
{
public:
    virtual ~State() = default;

    virtual void handleEvent(const sf::Event &event) = 0;
    virtual void update(sf::Time deltaTime, sf::RenderWindow &window) = 0;
    virtual void render(sf::RenderWindow &window) = 0;
};

#endif // STATE_H
