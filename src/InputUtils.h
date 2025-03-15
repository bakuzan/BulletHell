#ifndef INPUTUTILS_H
#define INPUTUTILS_H

#include <SFML/Window/Keyboard.hpp>
#include <vector>

namespace InputUtils
{
    bool isAnyKeyPressed(const std::vector<sf::Keyboard::Key> &keys);
    bool isAnyKeyPressed(sf::Keyboard::Key pressedKey, const std::vector<sf::Keyboard::Key> &keys);
}

#endif // INPUTUTILS_H
