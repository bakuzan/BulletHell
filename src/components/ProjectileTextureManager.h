#ifndef PROJECTILETEXTUREMANAGER_H
#define PROJECTILETEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "ProjectileType.h"

class ProjectileTextureManager
{
private:
    std::unordered_map<ProjectileType, sf::IntRect> textureRects;

public:
    ProjectileTextureManager();
    ~ProjectileTextureManager();

    const sf::IntRect &getTextureRect(ProjectileType type) const;
};

#endif // PROJECTILETEXTUREMANAGER_H
