#include "GameState.h"
#include "InputUtils.h"
#include "MenuState.h"

MenuState::MenuState(GameData &data, StateManager &manager, sf::RenderWindow &window)
    : gameData(data), stateManager(manager), window(window)
{
    // Load font
    if (!font.loadFromFile("resources/fonts/PressStart2P-Regular.ttf"))
    {
        throw std::runtime_error("Failed to load font");
    }

    // Setup title
    gameTitle.setFont(font);
    gameTitle.setString("Bullet Hell");
    gameTitle.setCharacterSize(48);
    gameTitle.setFillColor(sf::Color::White);

    // Set up the "New Game" button
    newGameButton.setFont(font);
    newGameButton.setString("New Game");
    newGameButton.setCharacterSize(36);
    newGameButton.setFillColor(sf::Color::White);

    // Set up the "Quit" button
    quitButton.setFont(font);
    quitButton.setString("Quit");
    quitButton.setCharacterSize(36);
    quitButton.setFillColor(sf::Color::White);

    // Highlight the first button initially
    newGameButton.setFillColor(sf::Color::Yellow);

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
    sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);
    bool isMouseOverNewGame = newGameButton.getGlobalBounds().contains(worldMousePos);
    bool isMouseOverQuit = quitButton.getGlobalBounds().contains(worldMousePos);

    // Check button hovered and highlight
    if (isMouseOverNewGame)
    {
        selectedButtonIndex = 0;
    }
    else if (isMouseOverQuit)
    {
        selectedButtonIndex = 1;
    }

    if (event.type == sf::Event::KeyPressed)
    {
        auto keyCode = event.key.code;
        if (!isMouseOverNewGame && !isMouseOverQuit)
        {
            if (InputUtils::isAnyKeyPressed(keyCode, {sf::Keyboard::Up, sf::Keyboard::W}))
            {
                selectedButtonIndex = (selectedButtonIndex - 1 + 2) % 2;
            }
            else if (InputUtils::isAnyKeyPressed(keyCode, {sf::Keyboard::Down, sf::Keyboard::S}))
            {
                selectedButtonIndex = (selectedButtonIndex + 1) % 2;
            }
        }

        if (keyCode == sf::Keyboard::Enter)
        {
            if (selectedButtonIndex == 0)
            {
                onNewGameClick();
            }
            else if (selectedButtonIndex == 1)
            {
                window.close();
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed &&
             event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

        if (newGameButton.getGlobalBounds().contains(worldMousePos))
        {
            onNewGameClick();
        }
        else if (quitButton.getGlobalBounds().contains(worldMousePos))
        {
            window.close();
        }
    }
    else if (event.type == sf::Event::Resized)
    {
        updateMenuItemPositions();
    }

    // Update button colors based on selection
    newGameButton.setFillColor(selectedButtonIndex == 0 ? sf::Color::Yellow : sf::Color::White);
    quitButton.setFillColor(selectedButtonIndex == 1 ? sf::Color::Yellow : sf::Color::White);
}

void MenuState::update(sf::Time deltaTime, sf::RenderWindow &window)
{
}

void MenuState::render(sf::RenderWindow &window)
{
    window.draw(background);
    window.draw(gameTitle);
    window.draw(newGameButton);
    window.draw(quitButton);
}

// Privates
void MenuState::updateMenuItemPositions()
{
    sf::View view = window.getView();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    // Resize background
    background.setSize(viewSize);
    background.setPosition(viewCenter - viewSize / 2.0f);

    float buttonX = viewCenter.x;

    // Set positions relative to the view's center and size
    newGameButton.setPosition(buttonX - newGameButton.getGlobalBounds().width / 2.0f,
                              viewCenter.y - 100.0f);

    quitButton.setPosition(buttonX - quitButton.getGlobalBounds().width / 2.0f,
                           viewCenter.y);

    gameTitle.setPosition(25.0f, 25.0f);
}

void MenuState::onNewGameClick()
{
    stateManager.changeState(std::make_unique<GameState>(gameData, stateManager, window));
}