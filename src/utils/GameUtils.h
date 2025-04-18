#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <SFML/Graphics.hpp>

#include "constants/Direction.h"

namespace GameUtils
{
    sf::Vector2f normaliseVector(const sf::Vector2f &vector);

    float rotateTowards(
        sf::Sprite &sprite,
        const sf::Vector2f &sourcePosition,
        const sf::Vector2f &targetPosition,
        float alignmentOffset = 0.0f);

    const sf::Vector2f GetRandomPositionOnSide(
        const sf::View &view,
        Direction side,
        float offset);

    std::string formatScoreText(int score);
};

#endif // GAMEUTILS_H
