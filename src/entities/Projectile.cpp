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

Projectile::~Projectile()
{
    // Destructor
}

// Publics

void Projectile::update(sf::Time deltaTime)
{
    sprite.move(velocity * deltaTime.asSeconds()); // Move the bullet

    float angle = std::atan2(velocity.y, velocity.x) * 180.f / 3.14159f; // Convert radians to degrees
    sprite.setRotation(angle + 90.0f);                                   // Add offset due to position in the spritesheet
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
        return ProjectileOrigin::PLAYER;
    case ProjectileType::BULLET_ALIEN:
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

bool Projectile::isOffScreen(sf::RenderWindow &window)
{
    sf::View view = window.getView();
    sf::Vector2f viewSize = view.getSize();

    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::FloatRect viewBounds(view.getCenter() - viewSize / 2.f, viewSize);
    return !viewBounds.intersects(bounds);
}
