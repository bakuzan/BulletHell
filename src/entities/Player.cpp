#include <cmath>

#include "Player.h"
#include "constants/Constants.h"
#include "utils/GameUtils.h"
#include "utils/InputUtils.h"

Player::Player(const sf::Texture &texture, sf::IntRect textureRect)
    : lastDirectionMoved(Direction::NONE), shoot(false)
{
    sprite.setTexture(texture);
    sprite.setTextureRect(textureRect);
    sprite.setScale(0.3f, 0.3f);
    sprite.setOrigin(Constants::SPRITE_WIDTH_PLAYER / 2.0f, Constants::SPRITE_HEIGHT_PLAYER / 2.0f);
    sprite.setPosition(100.0f, 100.0f);

    reset();
}

Player::~Player()
{
    // Destructor
}

// Publics

void Player::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        shoot = true;
    }

    if (InputUtils::isAnyKeyPressed({sf::Keyboard::Space}))
    {
        shoot = true;
    }
}

void Player::update(float deltaTime,
                    sf::RenderWindow &window)
{
    move(deltaTime);

    // Turn the player to face the mouse position
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);
    sf::Vector2f currentPlayerPos = sprite.getPosition();

    GameUtils::rotateTowards(
        sprite,
        currentPlayerPos,
        worldMousePosition,
        rotationOffset);
}

void Player::render(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

void Player::reset()
{
    health = 100.0f;
    shoot = false;
}

std::optional<ProjectileData> Player::getShootData()
{
    if (shoot)
    {
        shoot = false;

        // Get current rotation and convert to radians
        float angleDegrees = sprite.getRotation() - rotationOffset;
        float angleRadians = angleDegrees * (M_PI / 180.0f);

        // Calculate the bullet's velocity based on the player's rotation
        sf::Vector2f direction(std::cos(angleRadians), std::sin(angleRadians));
        sf::Vector2f bulletVelocity = direction * (Constants::PROJECTILE_SPEED_BULLET);

        // Calculate bullet spawn position (front-center of the player)
        float playerScale = sprite.getScale().x;                                                  // Assume it is the same both x/y
        float bulletWidth = 15.0f;                                                                // (size is the sprite size (300) * sprite scale (0.05))
        float bulletHeight = 25.0f;                                                               // (size is the sprite size (500) * sprite scale (0.05))
        sf::Vector2f offset = direction * ((Constants::SPRITE_WIDTH_PLAYER * playerScale) / 2.f); // Move to front
        sf::Vector2f rotatedCenterOffset = sf::Vector2f(
            -bulletWidth / 2.f * std::cos(angleRadians) + bulletHeight / 2.f * std::sin(angleRadians),
            -bulletWidth / 2.f * std::sin(angleRadians) - bulletHeight / 2.f * std::cos(angleRadians));

        // Calculate final spawn position
        sf::Vector2f spawnPosition = sprite.getPosition() + offset + rotatedCenterOffset;

        return ProjectileData(
            ProjectileType::BULLET,
            spawnPosition,
            bulletVelocity,
            Constants::PROJECTILE_DAMAGE_BULLET);
    }

    return std::nullopt;
}

const sf::Sprite &Player::getSprite() const
{
    return sprite;
}

const float Player::getHealth() const
{
    return health;
}

void Player::updateHealth(float adjustment)
{
    health = std::max(0.0f, health + adjustment); // Clamp at 0
}

const Direction Player::getLastDirectionMoved() const
{
    return lastDirectionMoved;
}

// Privates

void Player::move(float deltaTime)
{
    const float playerSpeed = Constants::BASE_PLAYER_SPEED;
    if (InputUtils::isAnyKeyPressed({sf::Keyboard::Left, sf::Keyboard::A}))
    {
        sprite.move(-playerSpeed * deltaTime, 0);
        lastDirectionMoved = Direction::LEFT;
    }
    if (InputUtils::isAnyKeyPressed({sf::Keyboard::Right, sf::Keyboard::D}))
    {
        sprite.move(playerSpeed * deltaTime, 0);
        lastDirectionMoved = Direction::RIGHT;
    }
    if (InputUtils::isAnyKeyPressed({sf::Keyboard::Up, sf::Keyboard::W}))
    {
        sprite.move(0, -playerSpeed * deltaTime);
        lastDirectionMoved = Direction::UP;
    }
    if (InputUtils::isAnyKeyPressed({sf::Keyboard::Down, sf::Keyboard::S}))
    {
        sprite.move(0, playerSpeed * deltaTime);
        lastDirectionMoved = Direction::DOWN;
    }
}