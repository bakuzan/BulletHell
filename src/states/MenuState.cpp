#include "Constants.h"
#include "GameState.h"
#include "utils/InputUtils.h"
#include "MenuState.h"

MenuState::MenuState(GameData &data, StateManager &manager, sf::RenderWindow &win)
    : gameData(data), stateManager(manager), window(win)
{
    buttonSpacing = Constants::BUTTON_HEIGHT + 10.f;
    sf::View view = window.getView();
    sf::Vector2f center(view.getCenter());

    // Setup title
    gameTitle.setFont(gameData.gameFont);
    gameTitle.setString("Bullet Hell");
    gameTitle.setCharacterSize(48);
    gameTitle.setFillColor(sf::Color::White);

    // Add buttons
    buttons.emplace_back(data.gameFont, "New Game", sf::Vector2f(center.x - 100.f, center.y - 60.f),
                         [this]()
                         { stateManager.changeState(std::make_unique<GameState>(gameData, stateManager, window)); });
    buttons.emplace_back(data.gameFont, "Exit", sf::Vector2f(center.x - 100.f, center.y + buttonSpacing),
                         [this]()
                         { window.close(); });

    // Set up the background
    background.setSize(sf::Vector2f(800, 600));
    background.setFillColor(sf::Color(31, 31, 31));

    // To ensure positioning is updated relative to window resizing
    updateMenuItemPositions();
}

MenuState::~MenuState()
{
    // Destructor
}

// Publics

void MenuState::handleEvent(const sf::Event &event)
{
    InputUtils::handleButtonEvent(event, buttons, window, selectedButtonIndex);

    if (event.type == sf::Event::Resized)
    {
        updateMenuItemPositions();
    }
}

void MenuState::update(sf::Time deltaTime, sf::RenderWindow &window)
{
}

void MenuState::render(sf::RenderWindow &window)
{
    window.draw(background);
    window.draw(gameTitle);

    for (const auto &button : buttons)
    {
        button.render(window);
    }
}

// Privates
void MenuState::updateMenuItemPositions()
{
    sf::View view = window.getView();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    background.setSize(viewSize);
    background.setPosition(viewCenter - viewSize / 2.0f);

    gameTitle.setPosition(viewCenter.x - viewSize.x / 2.f + 25.f,
                          viewCenter.y - viewSize.y / 2.f + 25.f);

    float offsetX = Constants::BUTTON_WIDTH / 2.0f;
    buttons[0].setPosition(sf::Vector2f(viewCenter.x - offsetX, viewCenter.y - (buttonSpacing / 2.0f)));
    buttons[1].setPosition(sf::Vector2f(viewCenter.x - offsetX, viewCenter.y + (buttonSpacing / 2.0f)));
}
