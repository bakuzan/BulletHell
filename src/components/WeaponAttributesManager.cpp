#include "constants/Constants.h"
#include "constants/ProjectileType.h"
#include "WeaponAttributesManager.h"

WeaponAttributesManager::WeaponAttributesManager()
{
  weaponAttributes = {
      // Player
      {WeaponType::DOUBLE_SHOT,
       {ProjectileType::DOUBLE_SHOT,
        Constants::PROJECTILE_DAMAGE_BULLET,
        Constants::PROJECTILE_SPEED_BULLET,
        {15.f, 25.f}}},
      {WeaponType::LAZER,
       {ProjectileType::LAZER,
        Constants::PROJECTILE_DAMAGE_LAZER,
        Constants::PROJECTILE_SPEED_LAZER,
        {15.f, 5000.f}}},
      {WeaponType::MISSILE,
       {ProjectileType::MISSILE,
        Constants::PROJECTILE_DAMAGE_MISSILE,
        Constants::PROJECTILE_SPEED_MISSILE,
        {15.f, 25.f}}},
      {WeaponType::BASIC,
       {ProjectileType::BULLET,
        Constants::PROJECTILE_DAMAGE_BULLET,
        Constants::PROJECTILE_SPEED_BULLET,
        {15.f, 25.f}}},

      // Enemy
      {WeaponType::ALIEN_BASIC,
       {ProjectileType::BULLET_ALIEN,
        Constants::PROJECTILE_DAMAGE_BULLET_ALIEN,
        Constants::PROJECTILE_SPEED_BULLET_ALIEN,
        {15.f, 10.f}}}};

  weaponTimeouts = {
      {WeaponType::DOUBLE_SHOT, 12.0f},
      {WeaponType::LAZER, 8.0f},
      {WeaponType::MISSILE, 6.0f},
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

const float &WeaponAttributesManager::getTimeout(WeaponType weapon) const
{
  return weaponTimeouts.at(weapon);
}