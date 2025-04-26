#include <iomanip>
#include <memory>

#include "constants/Constants.h"
#include "utils/InputUtils.h"
#include "utils/GameUtils.h"
#include "GameState.h"
#include "MenuState.h"
#include "GameOverState.h"

GameOverState::GameOverState(GameData &data, StateManager &manager, sf::RenderWindow &win)
    : gameData(data), stateManager(manager), window(win),
      isAskingForPlayerName(false),
      playerHasHighScore(false),
      playerName(""),
      playerScore(0)
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

    // Handle high scoring!
    highScoreManager.loadFromFile("highscores.txt");
    isAskingForPlayerName = playerHasHighScore = checkIfHighScore();

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
    if (isAskingForPlayerName)
    {
        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode == '\b' &&
                !playerName.empty())
            {
                playerName.pop_back();
            }
            else if (event.text.unicode < 128 &&
                     playerName.size() < 15)
            {
                playerName += static_cast<char>(event.text.unicode);
            }
        }

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Enter)
        {
            playerScore = calculateFinalScore();
            highScoreManager.addScore(playerName, playerScore);
            highScoreManager.saveToFile("highscores.txt");
            isAskingForPlayerName = false;
        }
    }
    else
    {
        InputUtils::handleButtonEvent(event, buttons, window, selectedButtonIndex);
    }

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

    if (isAskingForPlayerName)
    {
        // TODO Fix both positions to be view aware
        sf::Text prompt("Enter your name: ", gameData.gameFont, 48);
        prompt.setPosition(100, 200);
        prompt.setFillColor(sf::Color::Yellow);

        sf::Text input(playerName, gameData.gameFont, 48);
        input.setPosition(100, 250);
        input.setFillColor(sf::Color::White);

        window.draw(prompt);
        window.draw(input);
    }
    else
    {
        displayHighScores(highScoreManager.getHighScores(), window);

        for (const auto &button : buttons)
        {
            button.render(window);
        }
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

void GameOverState::displayHighScores(const std::vector<HighScore> &scores, sf::RenderWindow &window)
{
    sf::View view = window.getView();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    for (size_t i = 0; i < scores.size(); ++i)
    {
        const HighScore &entry = scores[i];

        std::tm *timeinfo = std::localtime(&entry.timestamp);
        std::ostringstream timeStream;
        timeStream << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");

        // Combine name, score, and timestamp into a display string
        std::string displayString = entry.name + " " + std::to_string(entry.score);
        // TODO
        // 25 - (name length + score.length (in display format))
        // use number to create ... between name and score.

        if (playerHasHighScore &&
            entry.name == playerName &&
            entry.score == playerScore)
        {
            displayString = "→ " + displayString; // TODO fix this ->
        }

        sf::Text text(displayString, gameData.gameFont, 36);
        text.setPosition(viewCenter.x - text.getGlobalBounds().width / 2.0f,
                         viewCenter.y - viewSize.y / 2.0f + gameOverText.getGlobalBounds().height + finalScoreText.getGlobalBounds().height + (buttonSpacing * 3.0f) + i * 50.0f);
        text.setFillColor(sf::Color::Yellow);
        window.draw(text);
    }
}

bool GameOverState::checkIfHighScore()
{
    auto &highscores = highScoreManager.getHighScores();
    if (highscores.size() < highScoreManager.maxHighScores)
    {
        return true;
    }

    int finalScore = calculateFinalScore();
    bool isOnLeaderBoard = false;

    for (const auto &score : highscores)
    {
        if (finalScore > score.score)
        {
            isOnLeaderBoard = true;
            break;
        }
    }

    return isOnLeaderBoard;
}

int GameOverState::calculateFinalScore()
{
    return gameData.getScore();
}
