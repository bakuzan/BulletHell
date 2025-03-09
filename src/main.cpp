#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>

#include "Constants.h"

void LoadWindowIcon(sf::Window &window)
{
    // Load the icon
    sf::Image icon;
    if (!icon.loadFromFile("resources/icon.png"))
    {
        exit(1); // Handle error
    }

    // Flip the icon by 180 degrees (invert both horizontally and vertically)
    sf::Vector2u iconSize = icon.getSize();
    sf::Image flippedIcon;
    flippedIcon.create(iconSize.x, iconSize.y);

    for (unsigned int x = 0; x < iconSize.x; ++x)
    {
        for (unsigned int y = 0; y < iconSize.y; ++y)
        {
            // Mirror pixels both horizontally and vertically
            sf::Color pixel = icon.getPixel(x, y);
            flippedIcon.setPixel(iconSize.x - x - 1, iconSize.y - y - 1, pixel);
        }
    }

    // Set the flipped icon
    window.setIcon(flippedIcon.getSize().x, flippedIcon.getSize().y, flippedIcon.getPixelsPtr());
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
    backgroundTexture.setRepeated(true);

    sf::RectangleShape background(sf::Vector2f(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT));
    background.setTexture(&backgroundTexture);

    // Entities
    sf::Texture spaceshipsTexture;
    if (!spaceshipsTexture.loadFromFile("resources/spaceships_brighter.png"))
    {
        std::cerr << "Failed to load spaceships texture!" << std::endl;
    }

    // Player
    sf::Sprite player;
    player.setTexture(spaceshipsTexture);
    player.setTextureRect(sf::IntRect(3 * Constants::SPRITE_SIZE, 0, Constants::SPRITE_SIZE, Constants::SPRITE_SIZE));
    player.setScale(0.25f, 0.25f);
    player.setOrigin(Constants::SPRITE_SIZE / 2.f, Constants::SPRITE_SIZE / 2.f);
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

                // Resize the background to match the new window size
                background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));

                // Adjust texture rect for seamless tiling
                background.setTextureRect(sf::IntRect(
                    view.getCenter().x - view.getSize().x / 2.0f,
                    view.getCenter().y - view.getSize().y / 2.0f,
                    window.getSize().x,
                    window.getSize().y));

                break;
            }
            default:
                break;
            }
        }

        // Turn the player to face the mouse location
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);

        // Calculate the angle between the player and mouse
        sf::Vector2f currentPlayerPos = player.getPosition();
        float dx = worldMousePosition.x - currentPlayerPos.x;
        float dy = worldMousePosition.y - currentPlayerPos.y;
        float angle = std::atan2(dy, dx) * 180.f / 3.14159f; // Convert to degrees
        player.setRotation(angle + 270.f);                   // Add 90 degrees to align sprite properly

        const float playerSpeed = Constants::BASE_PLAYER_SPEED;
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

        // Set the background sprite position
        sf::Vector2f viewPos = view.getCenter() - view.getSize() / 2.0f;
        background.setPosition(viewPos);
        background.setTextureRect(sf::IntRect(
            viewPos.x, viewPos.y,
            view.getSize().x, view.getSize().y));

        window.clear(sf::Color(31, 31, 31));
        window.draw(background);
        window.draw(player);

        // Display
        window.display();
    }

    return 0;
}
