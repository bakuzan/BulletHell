#include <SFML/Graphics.hpp>

#include "Constants.h"
#include "GameUtils.h"

void LoadWindowIcon(sf::Window &window)
{
    // Load the icon
    sf::Image icon;
    if (!icon.loadFromFile("resources/lightbulb-icon.png"))
    {
        exit(1);
    }

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Setup Test");
    sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT));

    LoadWindowIcon(window);

    // Objects
    sf::Clock gameClock;

    // World boundary
    sf::Vector2f worldSize(Constants::WORLD_WIDTH, Constants::WORLD_HEIGHT);
    sf::RectangleShape worldBoundary(sf::Vector2f(worldSize.x - 10.0f, worldSize.y - 10.0f));
    worldBoundary.setFillColor(sf::Color::Transparent);
    worldBoundary.setOutlineColor(sf::Color::Green);
    worldBoundary.setOutlineThickness(5.0f);
    worldBoundary.setPosition(5.0f, 5.0f);

    // Player
    sf::RectangleShape player(sf::Vector2f(50.0f, 50.0f));
    player.setFillColor(sf::Color(128, 0, 128));

    while (window.isOpen())
    {
        // Timing
        float deltaTime = gameClock.restart().asSeconds();

        // Input
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
            {
                // Maintain the height of the view to match the window height
                float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
                view.setSize(Constants::VIEW_HEIGHT * aspectRatio, Constants::VIEW_HEIGHT);
                break;
            }
            default:
                break;
            }
        }

        const float playerSpeed = 200.0f; // Pixels per second
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            player.move(-playerSpeed * deltaTime, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            player.move(playerSpeed * deltaTime, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            player.move(0.0f, -playerSpeed * deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            player.move(0.0f, playerSpeed * deltaTime);
        }

        // Clamp the player's position to the world boundaries
        ClampPlayerToWorld(player);

        // Logic

        // Draw + Display
        window.clear(sf::Color(31, 31, 31));

        UpdateView(view,
                   player.getPosition(),
                   worldSize,
                   sf::Vector2f(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT));
        window.setView(view);

        // Draw objects
        window.draw(worldBoundary);
        window.draw(player);

        // Display
        window.display();
    }

    return 0;
}
