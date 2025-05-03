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

void HealthBar::render(sf::RenderWindow &window)
{
    // Draw the border
    window.draw(borderSprite);

    // Draw the filling(s)
    sf::Vector2u fillingSpriteSize = fillingSprite.getTexture()->getSize();
    int availableFillings = 4;

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

        window.draw(fillingSprite);
    }
}

void HealthBar::setHealth(float health)
{
    currentHealth = std::max(0.0f, std::min(health, absoluteMaxHealth)); // Clamp between 0 and maxHealth
}

void HealthBar::setPosition(float x, float y)
{
    borderSprite.setPosition(x, y);
    fillingSprite.setPosition(x, y);
}

void HealthBar::scaleBasedOnTargetWidth(float targetWidth)
{
    float originalWidth = borderSprite.getTexture()->getSize().x;
    float scaleFactor = targetWidth / originalWidth;

    borderSprite.setScale(scaleFactor, 2.0f);
    fillingSprite.setScale(scaleFactor, 2.0f);
}