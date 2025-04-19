#include "HealthBar.h"

HealthBar::HealthBar(
    const sf::Texture &borderTexture, const sf::Texture &fillingTexture,
    float maxHealth,
    float initHealth)
    : absoluteMaxHealth(maxHealth), singleBarHealth(initHealth), currentHealth(initHealth)
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
    // Draw the border
    borderSprite.setPosition(view.getCenter().x - view.getSize().x / 2.0f + 20.0f,
                             view.getCenter().y - view.getSize().y / 2.0f + 20.0f);
    window.draw(borderSprite);

    // Draw the filling(s)
    sf::Vector2u fillingSpriteSize = fillingSprite.getTexture()->getSize();
    int availableFillings = 4;

    // int scaledWidth = static_cast<int>(fillingSpriteSize.x * fillingSprite.getScale().x);
    // int scaledHeight = static_cast<int>(fillingSpriteSize.y * fillingSprite.getScale().y / availableFillings);

    // Calculate how many full bars and the percentage of the next bar
    int fullBars = static_cast<int>(currentHealth / singleBarHealth);
    float remainingHealth = currentHealth - fullBars * singleBarHealth;
    float remainingPercentage = remainingHealth / singleBarHealth;

    // Draw each full bar (move down the sheet for each filling)
    for (int i = 0; i < fullBars; ++i)
    {
        fillingSprite.setTextureRect(sf::IntRect(
            0,
            i * fillingSpriteSize.y / availableFillings,
            fillingSpriteSize.x,
            fillingSpriteSize.y / availableFillings));

        fillingSprite.setPosition(borderSprite.getPosition());
        window.draw(fillingSprite);
    }

    if (remainingHealth > 0.0f)
    {
        int partialWidth = static_cast<int>((fillingSpriteSize.x) * remainingPercentage);
        fillingSprite.setTextureRect(sf::IntRect(
            0,
            fullBars * fillingSpriteSize.y / availableFillings,
            partialWidth,
            fillingSpriteSize.y / availableFillings));

        fillingSprite.setPosition(borderSprite.getPosition());
        window.draw(fillingSprite);
    }
}

void HealthBar::setHealth(float health)
{
    currentHealth = std::max(0.0f, std::min(health, absoluteMaxHealth)); // Clamp between 0 and maxHealth
}
