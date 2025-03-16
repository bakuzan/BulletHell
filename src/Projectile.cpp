#include "Projectile.h"

Projectile::Projectile(float x, float y, float width, float height, sf::Vector2f velocity)
    : velocity(velocity)
{
    shape.setPosition(x, y);
    shape.setSize({width, height});
    shape.setFillColor(sf::Color::Yellow); // Make it visually distinct
}

Projectile::~Projectile()
{
    // Destructor
}

// Publics

void Projectile::update(sf::Time deltaTime)
{
    shape.move(velocity * deltaTime.asSeconds()); // Move the bullet
}

void Projectile::render(sf::RenderWindow &window) const
{
    window.draw(shape);
}

bool Projectile::isOffScreen(sf::RenderWindow &window)
{
    sf::View view = window.getView();
    sf::Vector2f viewSize = view.getSize();

    sf::FloatRect bounds = shape.getGlobalBounds();
    sf::FloatRect viewBounds(view.getCenter() - viewSize / 2.f, viewSize);
    return !viewBounds.intersects(bounds);
}
