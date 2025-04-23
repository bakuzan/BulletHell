#pragma once

#include "constants/ProjectileType.h"
#include "ProjectileAttributes.h"

struct WeaponAttributes
{
    ProjectileType projectileType;
    float damage;
    float speed;
    ProjectileAttributes projectileAttrs;
};