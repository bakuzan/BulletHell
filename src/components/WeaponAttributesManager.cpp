#include "constants/Constants.h"
#include "constants/ProjectileType.h"
#include "WeaponAttributesManager.h"

WeaponAttributesManager::WeaponAttributesManager()
{
    weaponAttributes = {
        // Player
        {WeaponType::DOUBLE_SHOT, {ProjectileType::DOUBLE_SHOT, Constants::PROJECTILE_DAMAGE_BULLET, Constants::PROJECTILE_SPEED_BULLET}},
        {WeaponType::LAZER, {ProjectileType::LAZER, Constants::PROJECTILE_DAMAGE_LAZER, Constants::PROJECTILE_SPEED_LAZER}},
        {WeaponType::MISSILE, {ProjectileType::MISSILE, Constants::PROJECTILE_DAMAGE_MISSILE, Constants::PROJECTILE_SPEED_MISSILE}},
        {WeaponType::BASIC, {ProjectileType::BULLET, Constants::PROJECTILE_DAMAGE_BULLET, Constants::PROJECTILE_SPEED_BULLET}},
    };
}

WeaponAttributesManager::~WeaponAttributesManager()
{
    // Destructor
}

// Publics

const WeaponAttributes &WeaponAttributesManager::getAttributes(WeaponType weapon) const
{
    return weaponAttributes.at(weapon);
}