#ifndef UPGRADEBOX_H
#define UPGRADEBOX_H

#include <SFML/Graphics.hpp>

#include "constants/UpgradeBoxType.h"

class UpgradeBox
{
private:
    sf::Sprite sprite;
    UpgradeBoxType type;

    float lifeTime;

public:
    UpgradeBox(UpgradeBoxType type,
               const sf::Texture &texture, sf::IntRect textureRect,
               sf::Vector2f spawnPosition, float initialLifetime);
    ~UpgradeBox();

    void update(sf::Time deltaTime);
    void render(sf::RenderWindow &window) const;

    const sf::Sprite &getSprite() const;
    const UpgradeBoxType &getType() const;
    const bool isExpired() const;
};

#endif // UPGRADEBOX_H
