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
}