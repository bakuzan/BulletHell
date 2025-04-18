#ifndef ENEMYTEXTUREMANAGER_H
#define ENEMYTEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "constants/EnemyType.h"

class EnemyTextureManager
{
private:
    std::unordered_map<EnemyType, sf::IntRect> textureRects;

public:
    EnemyTextureManager();
    ~EnemyTextureManager();

    const sf::IntRect &getTextureRect(EnemyType type) const;
};

#endif // ENEMYTEXTUREMANAGER_H
