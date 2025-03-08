#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/View.hpp>

void ClampPlayerToWorld(sf::RectangleShape &player);
void UpdateView(sf::View &view,
                const sf::Vector2f &playerPos,
                const sf::Vector2f &worldSize,
                const sf::Vector2f &viewSize);

#endif // GAMEUTILS_H
