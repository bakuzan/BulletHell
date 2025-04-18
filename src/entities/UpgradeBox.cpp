#include "constants/Constants.h"
#include "UpgradeBox.h"

UpgradeBox::UpgradeBox(UpgradeBoxType boxType,
                       const sf::Texture &texture, sf::IntRect textureRect,
                       sf::Vector2f spawnPosition, float initialLifetime)
    : type(boxType), lifeTime(initialLifetime)
{
    sprite.setTexture(texture);
    sprite.setTextureRect(textureRect);
    sprite.setPosition(spawnPosition);
}

UpgradeBox::~UpgradeBox()
{
    // Destructor
}

// Publics

void UpgradeBox::update(sf::Time deltaTime)
{
    lifeTime -= deltaTime.asSeconds();
}

void UpgradeBox::render(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

const sf::Sprite &UpgradeBox::getSprite() const
{
    return sprite;
}

const bool UpgradeBox::isExpired() const
{
    return lifeTime <= 0.0f;
}