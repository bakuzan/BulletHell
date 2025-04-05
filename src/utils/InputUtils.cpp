#include "InputUtils.h"

namespace InputUtils
{
    bool isAnyKeyPressed(const std::vector<sf::Keyboard::Key> &keys)
    {
        for (const auto &key : keys)
        {
            if (sf::Keyboard::isKeyPressed(key))
            {
                return true;
            }
        }

        return false;
    }
    bool isAnyKeyPressed(sf::Keyboard::Key pressedKey, const std::vector<sf::Keyboard::Key> &keys)
    {
        for (const auto &key : keys)
        {
            if (pressedKey == key)
            {
                return true;
            }
        }

        return false;
    }

    void handleButtonEvent(const sf::Event &event,
                           std::vector<Button> &buttons,
                           sf::RenderWindow &window,
                           int &selectedButtonIndex)
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

        // Update button hover states
        for (size_t i = 0; i < buttons.size(); ++i)
        {
            buttons[i].onHover(i == selectedButtonIndex);
        }
    }
}