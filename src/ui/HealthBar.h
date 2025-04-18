#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <SFML/Graphics.hpp>

class HealthBar
{
private:
    sf::Sprite borderSprite;
    sf::Sprite fillingSprite;
    float currentHealth;
    float singleBarHealth;
    float absoluteMaxHealth;

public:
    HealthBar(const sf::Texture &borderTexture, const sf::Texture &fillingTexture,
              float maxHealth, float initHealth);
    ~HealthBar();

    void render(sf::RenderWindow &window, const sf::View &view);
    void setHealth(float health);
};

#endif // HEALTHBAR_H
