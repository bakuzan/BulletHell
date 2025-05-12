#include <cmath>

#include "Projectile.h"

Projectile::Projectile(ProjectileType type,
                       const sf::Texture &texture, sf::IntRect textureRect,
                       sf::Vector2f spawnPosition, sf::Vector2f velocity,
                       float damage)
    : velocity(velocity), type(type), damage(damage)
{
    sprite.setTexture(texture);
    sprite.setTextureRect(textureRect);
    sprite.setScale(0.05f, 0.05f);
    sprite.setPosition(spawnPosition);
}

// Publics

void Projectile::update(sf::Time deltaTime)
{
    sprite.move(velocity * deltaTime.asSeconds()); // Move the bullet

    float angle = std::atan2(velocity.y, velocity.x) * 180.f / M_PI; // Convert radians to degrees
    sprite.setRotation(angle + 90.0f);                               // Add offset due to position in the spritesheet
}

void Projectile::render(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

const ProjectileOrigin Projectile::getOrigin() const
{
    switch (type)
    {
    case ProjectileType::BULLET:
    case ProjectileType::DOUBLE_SHOT:
    case ProjectileType::LASER:
    case ProjectileType::MISSILE:
    case ProjectileType::MISSILE_DEBRIS:
        return ProjectileOrigin::PLAYER;
    case ProjectileType::ALIEN_BULLET:
    case ProjectileType::ALIEN_DEBRIS:
    case ProjectileType::ALIEN_LASER:
    case ProjectileType::ALIEN_SEEKER:
        return ProjectileOrigin::ENEMY;
    }

    throw std::logic_error("Unhandled ProjectileType in Projectile::getOrigin()");
}

const float Projectile::getDamageInflicts() const
{
    return damage;
}

const sf::Sprite &Projectile::getSprite() const
{
    return sprite;
}

const ProjectileType &Projectile::getType() const
{
    return type;
}

bool Projectile::isOutOfPlay(sf::RenderWindow &window)
{
    sf::View view = window.getView();
    sf::Vector2f viewSize = view.getSize();

    float bufferFactor = 0.1f;
    sf::Vector2f buffer = viewSize * bufferFactor;
    sf::FloatRect viewBounds(view.getCenter() - (viewSize / 2.f) - buffer,
                             viewSize + buffer * 2.f);

    return !viewBounds.intersects(sprite.getGlobalBounds()) &&
           isMovingAway(window);
}

// Protecteds

bool Projectile::isMovingAway(sf::RenderWindow &window)
{
    sf::View view = window.getView();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f projectilePos = sprite.getPosition();
    sf::Vector2f velocityDir = velocity;

    sf::Vector2f toViewCenter = viewCenter - projectilePos; // Direction toward screen center

    // Check if projectile is moving away from view
    return (velocityDir.x * toViewCenter.x + velocityDir.y * toViewCenter.y) < 0;
}
