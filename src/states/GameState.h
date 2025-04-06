#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>

#include "core/GameData.h"
#include "core/State.h"
#include "core/StateManager.h"
#include "entities/Enemy.h"
#include "ui/HealthBar.h"

class GameState : public State
{
private:
    GameData &gameData;
    StateManager &stateManager;
    sf::RenderWindow &window;
    sf::View view;

    sf::RectangleShape background;
    sf::Sprite player;

    HealthBar healthBar;
    sf::Text scoreText;

    bool shootProjectile;

private:
    void movePlayer(sf::Time &deltaTime);
    void aimAndShoot(sf::RenderWindow &window);

    void updateProjectiles(const sf::Time &deltaTime, sf::RenderWindow &window);

    std::unique_ptr<Enemy> spawnEnemy(
        EnemyType type,
        const sf::Texture &texture, sf::IntRect textureRect,
        sf::Vector2f spawnPosition, float speed);
    void spawnEnemies(float deltaTime, const sf::Texture &basicEnemyTexture);
    void updateEnemies(float deltaTime, const sf::Vector2f &playerPosition);
    void processEnemyShooting(float deltaTime, const sf::Vector2f &playerPosition);

    sf::Vector2f getRandomSpawnPosition(
        const sf::Vector2f &playerPosition,
        const sf::View &view);

    void renderScoreText(sf::RenderWindow &window, const sf::View &view);
    void updateScoreText(int score);

    void ensureBackgroundSizeIsLinkedToViewSize(
        const sf::Vector2f &viewPos,
        const sf::Vector2f &viewSize);

public:
    GameState(GameData &data, StateManager &manager, sf::RenderWindow &window);
    ~GameState();

    void handleEvent(const sf::Event &event) override;
    void update(sf::Time deltaTime, sf::RenderWindow &window) override;
    void render(sf::RenderWindow &window) override;
};

#endif // GAMESTATE_H
