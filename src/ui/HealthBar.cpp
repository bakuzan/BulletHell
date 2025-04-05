#include "HealthBar.h"

HealthBar::HealthBar(
    const sf::Texture &borderTexture, const sf::Texture &fillingTexture,
    float totalHealth)
    : totalHealth(totalHealth), currentHealth(totalHealth)
{
    borderSprite.setTexture(borderTexture);
    borderSprite.setScale(2.0f, 2.0f);
    fillingSprite.setTexture(fillingTexture);
    fillingSprite.setScale(2.0f, 2.0f);
}

HealthBar::~HealthBar()
{
    // Destructor
}

// Publics

void HealthBar::render(sf::RenderWindow &window, const sf::View &view)
{
    sf::Vector2u fillingSpriteSize = fillingSprite.getTexture()->getSize();
    float healthPercentage = currentHealth / totalHealth;
    int fillingWidth = static_cast<int>(healthPercentage * fillingSpriteSize.x);
    fillingSprite.setTextureRect(sf::IntRect(0, 0, fillingWidth, fillingSpriteSize.y));

    // Positioning relative to the view
    borderSprite.setPosition(view.getCenter().x - view.getSize().x / 2.0f + 20.0f,
                             view.getCenter().y - view.getSize().y / 2.0f + 20.0f);
    fillingSprite.setPosition(borderSprite.getPosition());

    window.draw(borderSprite);
    window.draw(fillingSprite);
}

void HealthBar::setHealth(float health)
{
    currentHealth = std::max(0.0f, std::min(health, totalHealth)); // Clamp between 0 and totalHealth
}
