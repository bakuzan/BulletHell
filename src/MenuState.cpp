#include "Constants.h"
#include "GameState.h"
#include "InputUtils.h"
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
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Up)
        {
            selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
            selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        }
        else if (event.key.code == sf::Keyboard::Enter)
        {
            buttons[selectedButtonIndex].trigger();
        }
    }
    else if (event.type == sf::Event::MouseMoved)
    {
        for (size_t i = 0; i < buttons.size(); ++i)
        {
            if (buttons[i].isMouseOver(mousePos, window))
            {
                selectedButtonIndex = static_cast<int>(i);
                break;
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed &&
             event.mouseButton.button == sf::Mouse::Left)
    {
        for (const auto &button : buttons)
        {
            if (button.isMouseOver(mousePos, window))
            {
                button.trigger();
            }
        }
    }

    // Update button hover state
    for (size_t i = 0; i < buttons.size(); ++i)
    {
        buttons[i].onHover(i == selectedButtonIndex);
    }

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
