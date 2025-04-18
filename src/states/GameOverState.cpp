#include <memory>

#include "constants/Constants.h"
#include "utils/InputUtils.h"
#include "utils/GameUtils.h"
#include "GameState.h"
#include "MenuState.h"
#include "GameOverState.h"

GameOverState::GameOverState(GameData &data, StateManager &manager, sf::RenderWindow &win)
    : gameData(data), stateManager(manager), window(win)
{
    buttonSpacing = 20.f;
    sf::View view = window.getView();
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f center(view.getCenter());

    // Configure background
    background.setSize(sf::Vector2f(viewSize.x - 20.0f, viewSize.y - 20.0f));
    background.setFillColor(sf::Color(0, 0, 0, 150));

    // Configure text
    gameOverText.setFont(gameData.gameFont);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(sf::Color::Red);

    finalScoreText.setFont(gameData.gameFont);
    finalScoreText.setString(GameUtils::formatScoreText(gameData.getScore()));
    finalScoreText.setCharacterSize(48);
    finalScoreText.setFillColor(sf::Color::Yellow);

    // Add buttons
    float buttonRowY = center.y + viewSize.y / 2.f - Constants::BUTTON_HEIGHT - buttonSpacing;

    buttons.emplace_back(data.gameFont, "New Game", sf::Vector2f(center.x - viewSize.x / 2.f + buttonSpacing, buttonRowY),
                         [this]()
                         {
                             gameData.reset();
                             stateManager.replaceStates(std::make_unique<GameState>(gameData, stateManager, window));
                         });
    buttons.emplace_back(data.gameFont, "Main Menu", sf::Vector2f(center.x - viewSize.x / 2.f + (buttonSpacing * 2.0f) + Constants::BUTTON_WIDTH, buttonRowY),
                         [this]()
                         {
                             gameData.reset();
                             stateManager.replaceStates(std::make_unique<MenuState>(gameData, stateManager, window));
                         });
    buttons.emplace_back(data.gameFont, "Exit", sf::Vector2f(center.x + viewSize.x / 2.f - Constants::BUTTON_WIDTH - buttonSpacing, buttonRowY),
                         [this]()
                         { window.close(); });

    // To ensure positioning is updated relative to window resizing
    updateMenuItemPositions();
}

GameOverState::~GameOverState()
{
    // Destructor
}

// Publics

void GameOverState::handleEvent(const sf::Event &event)
{
    InputUtils::handleButtonEvent(event, buttons, window, selectedButtonIndex);

    if (event.type == sf::Event::Resized)
    {
        updateMenuItemPositions();
    }
}

void GameOverState::update(sf::Time deltaTime, sf::RenderWindow &window)
{
    // No game logic when it is over
}

void GameOverState::render(sf::RenderWindow &window)
{
    window.draw(background);
    window.draw(gameOverText);
    window.draw(finalScoreText);

    for (const auto &button : buttons)
    {
        button.render(window);
    }
}

// Privates
void GameOverState::updateMenuItemPositions()
{
    sf::View view = window.getView();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f backgroundSize = background.getSize();

    background.setPosition(
        viewCenter.x - backgroundSize.x / 2.f,
        viewCenter.y - backgroundSize.y / 2.f);

    gameOverText.setPosition(
        viewCenter.x - (gameOverText.getGlobalBounds().width / 2.0f),
        viewCenter.y - viewSize.y / 2.0f + buttonSpacing);

    finalScoreText.setPosition(
        viewCenter.x - (finalScoreText.getGlobalBounds().width / 2.0f),
        viewCenter.y - viewSize.y / 2.0f + gameOverText.getGlobalBounds().height + (buttonSpacing * 2.0f));

    float buttonRowY = viewCenter.y + viewSize.y / 2.f - Constants::BUTTON_HEIGHT - buttonSpacing;
    buttons[0].setPosition(sf::Vector2f(viewCenter.x - viewSize.x / 2.f + buttonSpacing, buttonRowY));
    buttons[1].setPosition(sf::Vector2f(viewCenter.x - viewSize.x / 2.f + (buttonSpacing * 2.0f) + Constants::BUTTON_WIDTH, buttonRowY));
    buttons[2].setPosition(sf::Vector2f(viewCenter.x + viewSize.x / 2.f - Constants::BUTTON_WIDTH - buttonSpacing, buttonRowY));
}
