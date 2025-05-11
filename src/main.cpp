#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "utils/ErrorHandling.h"
#include "core/GameData.h"
#include "states/MenuState.h"
#include "core/StateManager.h"

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
    RegisterSignalHandlers();

    std::srand(std::time(0)); // Seed for rand

    sf::RenderWindow window(sf::VideoMode(800, 600), "Bullet Hell");
    LoadWindowIcon(window);

    GameData gameData;
    StateManager stateManager;

    // Push initial state (GameState)
    stateManager.pushState(std::make_unique<MenuState>(gameData, stateManager, window));

    sf::Clock clock;
    float accumulator = 0.0f;
    const float fixedTimeStep = 1.0f / 60.0f;

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
            stateManager.handleEvent(event);
        }

        // Update (Logic)
        sf::Time deltaTime = clock.restart();
        accumulator += deltaTime.asSeconds();

        while (accumulator >= fixedTimeStep)
        {
            stateManager.update(sf::seconds(fixedTimeStep), window);
            accumulator -= fixedTimeStep;
        }

        // Draw and Display
        window.clear();
        stateManager.render(window);
        window.display();
    }

    return 0;
}
