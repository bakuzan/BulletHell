#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>

#include "Enemy.h"
#include "GameData.h"
#include "State.h"
#include "StateManager.h"

class GameState : public State
{
private:
    GameData &gameData;
    StateManager &stateManager;
    sf::RenderWindow &window;

    sf::Texture backgroundTexture;
    sf::Texture spaceshipsTexture;
    sf::Texture projectileTexture;
    sf::RectangleShape background;
    sf::Sprite player;
    sf::View view;

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
    void updateEnemies(float deltaTime, sf::Vector2f playerPosition);

    sf::Vector2f getRandomSpawnPosition(const sf::Vector2f &playerPosition, const sf::RenderWindow &window);

public:
    GameState(GameData &data, StateManager &manager, sf::RenderWindow &window);
    ~GameState();

    void handleEvent(const sf::Event &event) override;
    void update(sf::Time deltaTime, sf::RenderWindow &window) override;
    void render(sf::RenderWindow &window) override;
};

#endif // GAMESTATE_H
