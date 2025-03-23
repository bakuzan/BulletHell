#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "GameData.h"
#include "MenuState.h"
#include "StateManager.h"

void LoadWindowIcon(sf::Window &window)
{
    // Load the icon
    sf::Image icon;
    if (!icon.loadFromFile("resources/icon.png"))
    {
        exit(1);
    }

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

int main()
{
    std::srand(std::time(0)); // Seed for rand

    sf::RenderWindow window(sf::VideoMode(800, 600), "Bullet Hell");
    LoadWindowIcon(window);

    GameData gameData;
    StateManager stateManager;

    // Push initial state (GameState)
    stateManager.pushState(std::make_unique<MenuState>(gameData, stateManager, window));

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Input
            if (stateManager.getCurrentState())
            {
                stateManager.getCurrentState()->handleEvent(event);
            }
        }

        // Update (Logic)
        sf::Time deltaTime = clock.restart();
        if (stateManager.getCurrentState())
        {
            stateManager.getCurrentState()->update(deltaTime, window);
        }

        // Draw and Display
        window.clear();
        if (stateManager.getCurrentState())
        {
            stateManager.getCurrentState()->render(window);
        }
        window.display();
    }

    return 0;
}
