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
      {WeaponType::LASER,
       {ProjectileType::LASER,
        Constants::PROJECTILE_DAMAGE_LASER,
        Constants::PROJECTILE_SPEED_LASER,
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
       {ProjectileType::ALIEN_BULLET,
        Constants::PROJECTILE_DAMAGE_ALIEN_BULLET,
        Constants::PROJECTILE_SPEED_ALIEN_BULLET,
        {15.f, 10.f}}},

      {WeaponType::ALIEN_SEEKER,
       {ProjectileType::ALIEN_SEEKER,
        Constants::PROJECTILE_DAMAGE_ALIEN_SEEKER,
        Constants::PROJECTILE_SPEED_ALIEN_SEEKER,
        {15.f, 25.f}}},

      {WeaponType::ALIEN_LASER,
       {ProjectileType::ALIEN_LASER,
        Constants::PROJECTILE_DAMAGE_ALIEN_LASER,
        Constants::PROJECTILE_SPEED_ALIEN_LASER,
        {15.f, 5000.f}}},

  };

  weaponTimeouts = {
      {WeaponType::DOUBLE_SHOT, 12.0f},
      {WeaponType::LASER, 8.0f},
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