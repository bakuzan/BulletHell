#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <SFML/Graphics.hpp>

#include "constants/Direction.h"
#include "constants/AudioId.h"
#include "constants/ProjectileType.h"
#include "data/SpawnData.h"
#include "data/WeaponAttributes.h"

namespace GameUtils
{
    sf::Vector2f normaliseVector(const sf::Vector2f &vector);

    float rotateTowards(
        sf::Sprite &sprite,
        const sf::Vector2f &sourcePosition,
        const sf::Vector2f &targetPosition,
        float alignmentOffset = 0.0f);

    SpawnData getSpawnDataForProjectileFromEntity(
        const sf::Sprite &sprite,
        const WeaponAttributes &weaponAttrs,
        float rotationOffset);

    const sf::Vector2f getRandomPositionOnSide(
        const sf::View &view,
        Direction side,
        float offset);

    const AudioId getAudioIdForProjectileType(ProjectileType projectileType);

    std::string formatScoreText(int score);
};

#endif // GAMEUTILS_H
