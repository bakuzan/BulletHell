#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <optional>

#include "constants/Direction.h"
#include "data/ProjectileData.h"
#include "Projectile.h"

class Player
{
private:
    sf::Sprite sprite;
    Direction lastDirectionMoved;
    float health;

    bool shoot;

public:
    static constexpr float rotationOffset = 270.0f;

private:
    void move(float deltaTime);

public:
    Player(const sf::Texture &texture, sf::IntRect textureRect);
    ~Player();

    void handleEvent(const sf::Event &event);
    void update(float deltaTime, sf::RenderWindow &window);
    void render(sf::RenderWindow &window) const;
    void reset();

    std::optional<ProjectileData> getShootData();
    const sf::Sprite &getSprite() const;
    const float getHealth() const;
    void updateHealth(float adjustment);
    const Direction getLastDirectionMoved() const;
};

#endif // PLAYER_H
