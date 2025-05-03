#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <optional>

#include "constants/Direction.h"
#include "constants/WeaponType.h"
#include "data/ProjectileData.h"
#include "ui/HealthBar.h"

class Player
{
private:
    sf::Sprite sprite;
    Direction lastDirectionMoved;
    float health;
    float initHealth;
    float maxHealth;
    HealthBar healthBar;

    bool shoot;
    float weaponTimeout;
    WeaponType weaponType;

public:
    static constexpr float rotationOffset = 270.0f;

private:
    void move(float deltaTime);

public:
    Player(const sf::Texture &borderTexture, const sf::Texture &fillingTexture,
           const sf::Texture &texture, sf::IntRect textureRect,
           float maxHealth, float initHealth);
    ~Player();

    void handleEvent(const sf::Event &event);
    void update(float deltaTime, sf::RenderWindow &window);
    void render(sf::RenderWindow &window);
    void reset();

    std::optional<ProjectileData> getShootData();
    const Direction getLastDirectionMoved() const;
    const sf::Sprite &getSprite() const;
    const float getHealth() const;
    void updateHealth(float adjustment);
    void setWeaponType(WeaponType type);
};

#endif // PLAYER_H
