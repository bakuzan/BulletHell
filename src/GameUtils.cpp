#include <algorithm>

#include "Constants.h"
#include "GameUtils.h"

void ClampPlayerToWorld(sf::RectangleShape &player)
{
    sf::Vector2f playerPosition = player.getPosition();
    sf::FloatRect playerBounds = player.getGlobalBounds();
    playerPosition.x = std::max(0.0f, std::min(playerPosition.x, Constants::WORLD_WIDTH - playerBounds.width));
    playerPosition.y = std::max(0.0f, std::min(playerPosition.y, Constants::WORLD_HEIGHT - playerBounds.height));
    player.setPosition(playerPosition);
}

void UpdateView(sf::View &view,
                const sf::Vector2f &playerPos,
                const sf::Vector2f &worldSize,
                const sf::Vector2f &viewSize)
{
    sf::Vector2f clampedCenter = playerPos; // Start by centering the view on the player
    sf::Vector2f halfViewSize = viewSize / 2.0f;

    clampedCenter.x = std::max(halfViewSize.x, std::min(clampedCenter.x, worldSize.x - halfViewSize.x));
    clampedCenter.y = std::max(halfViewSize.y, std::min(clampedCenter.y, worldSize.y - halfViewSize.y));

    view.setCenter(clampedCenter);
}
