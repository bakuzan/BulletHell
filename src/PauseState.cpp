#include <memory>

#include "Constants.h"
#include "InputUtils.h"
#include "MenuState.h"
#include "PauseState.h"

PauseState::PauseState(GameData &data, StateManager &manager, sf::RenderWindow &win)
    : gameData(data), stateManager(manager), window(win)
{
    buttonSpacing = Constants::BUTTON_HEIGHT + 10.f;
    sf::View view = window.getView();
    sf::Vector2f center(view.getCenter());

    // Configure background
    background.setSize(sf::Vector2f(300.f, 300.f));
    background.setFillColor(sf::Color(0, 0, 0, 150));

    // Configure pause text
    pauseText.setFont(gameData.gameFont);
    pauseText.setString("Paused");
    pauseText.setCharacterSize(36);
    pauseText.setFillColor(sf::Color::White);

    // Add buttons
    buttons.emplace_back(data.gameFont, "Resume", sf::Vector2f(center.x - 100.f, center.y - buttonSpacing),
                         [this]()
                         { stateManager.popState(); });
    buttons.emplace_back(data.gameFont, "Quit Game", sf::Vector2f(center.x - 100.f, center.y),
                         [this]()
                         {
                             gameData.reset();
                             stateManager.replaceStates(std::make_unique<MenuState>(gameData, stateManager, window));
                         });
    buttons.emplace_back(data.gameFont, "Exit", sf::Vector2f(center.x - 100.f, center.y + buttonSpacing),
                         [this]()
                         { window.close(); });

    // To ensure positioning is updated relative to window resizing
    updateMenuItemPositions();
}

PauseState::~PauseState()
{
    // Destructor
}

// Publics

void PauseState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed &&
        InputUtils::isAnyKeyPressed(event.key.code, {sf::Keyboard::Escape, sf::Keyboard::P}))
    {
        stateManager.popState(); // Resume the game by returning to GameState
    }

    InputUtils::handleButtonEvent(event, buttons, window, selectedButtonIndex);

    if (event.type == sf::Event::Resized)
    {
        updateMenuItemPositions();
    }
}

void PauseState::update(sf::Time deltaTime, sf::RenderWindow &window)
{
    // No game logic while paused
}

void PauseState::render(sf::RenderWindow &window)
{
    window.draw(background);
    window.draw(pauseText);

    for (const auto &button : buttons)
    {
        button.render(window);
    }
}

// Privates
void PauseState::updateMenuItemPositions()
{
    sf::View view = window.getView();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f backgroundSize = background.getSize();

    background.setPosition(
        viewCenter.x - backgroundSize.x / 2.f,
        viewCenter.y - backgroundSize.y / 2.f);

    pauseText.setPosition(
        background.getPosition().x + (backgroundSize.x - pauseText.getGlobalBounds().width) / 2.f,
        background.getPosition().y + 20.f);

    float offsetX = Constants::BUTTON_WIDTH / 2.0f;
    buttons[0].setPosition(sf::Vector2f(viewCenter.x - offsetX, viewCenter.y - buttonSpacing));
    buttons[1].setPosition(sf::Vector2f(viewCenter.x - offsetX, viewCenter.y));
    buttons[2].setPosition(sf::Vector2f(viewCenter.x - offsetX, viewCenter.y + buttonSpacing));
}