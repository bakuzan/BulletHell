#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>

#include "core/GameData.h"
#include "core/State.h"
#include "core/StateManager.h"
#include "entities/Enemy.h"
#include "ui/HealthBar.h"
#include "components/EnemySpawnManager.h"

class GameState : public State
{
private:
    GameData &gameData;
    StateManager &stateManager;
    sf::RenderWindow &window;
    sf::View view;

    EnemySpawnManager enemySpawnManager;

    sf::RectangleShape background;
    sf::Sprite player;

    HealthBar healthBar;
    sf::Text scoreText;

    bool shootProjectile;

private:
    void movePlayer(sf::Time &deltaTime);
    void aimAndShoot(sf::RenderWindow &window);

    void updateProjectiles(const sf::Time &deltaTime, sf::RenderWindow &window);

    void updateEnemies(float deltaTime, const sf::Vector2f &playerPosition);
    void processEnemyShooting(float deltaTime, const sf::Vector2f &playerPosition);

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
