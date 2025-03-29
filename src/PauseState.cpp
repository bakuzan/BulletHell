#include "InputUtils.h"
#include "PauseState.h"

PauseState::PauseState(GameData &data, StateManager &manager, sf::RenderWindow &window)
    : gameData(data), stateManager(manager), window(window)
{
    pauseText.setFont(gameData.gameFont);
    pauseText.setString("Game Paused");
    pauseText.setCharacterSize(48);
    pauseText.setFillColor(sf::Color::White);

    // Handle view-relative positioning
    sf::View view = window.getView();
    pauseText.setPosition(
        view.getCenter().x - pauseText.getGlobalBounds().width / 2.0f,
        view.getCenter().y - pauseText.getGlobalBounds().height / 2.0f);
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
}

void PauseState::update(sf::Time deltaTime, sf::RenderWindow &window)
{
    // No game logic while paused
}

void PauseState::render(sf::RenderWindow &window)
{
    window.draw(pauseText);
}