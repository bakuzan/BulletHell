#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <SFML/Graphics.hpp>
#include <string>

class HealthBar
{
private:
    sf::Sprite borderSprite;
    sf::Sprite fillingSprite;
    float currentHealth;
    float singleBarHealth;
    float absoluteMaxHealth;

    std::string name;

public:
    HealthBar(const sf::Texture &borderTexture, const sf::Texture &fillingTexture,
              float maxHealth, float initHealth,
              std::string barName);
    ~HealthBar();

    void render(sf::RenderWindow &window);
    void setHealth(float health);

    void setPosition(float x, float y);
    void scaleBasedOnTargetWidth(float targetWidth);
};

#endif // HEALTHBAR_H
