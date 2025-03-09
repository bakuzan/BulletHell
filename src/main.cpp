#include <SFML/Graphics.hpp>
#include <iostream>

#include "Constants.h"

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
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bullet Hell");
    sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT));

    LoadWindowIcon(window);

    // Objects
    sf::Clock gameClock;

    // Background
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("resources/background.png"))
    {
        // Handle error
        std::cerr << "Failed to load background image!" << std::endl;
    }

    sf::RectangleShape background(sf::Vector2f(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT));
    background.setTexture(&backgroundTexture);

    // Entities
    sf::Texture spaceshipsTexture;
    if (!spaceshipsTexture.loadFromFile("resources/spaceships.png"))
    {
        std::cerr << "Failed to load spaceships texture!" << std::endl;
    }

    // Player
    sf::Sprite player;
    player.setTexture(spaceshipsTexture);
    player.setTextureRect(sf::IntRect(3 * 128, 0, 128, 128));
    player.setScale(0.5f, 0.5f);
    player.setOrigin(64 / 2.f, 64 / 2.f);
    player.setPosition(100.0f, 100.0f);

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

        const float playerSpeed = 500.0f; // Pixels per second
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

        // Logic

        // Draw + Display
        view.setCenter(player.getPosition());
        window.setView(view);

        window.clear(sf::Color(31, 31, 31));
        window.draw(background);
        window.draw(player);

        // Display
        window.display();
    }

    return 0;
}
