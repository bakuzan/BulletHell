#ifndef WEAPONATTRIBUTESMANAGER_H
#define WEAPONATTRIBUTESMANAGER_H

#include <unordered_map>

#include "constants/WeaponType.h"
#include "data/WeaponAttributes.h"

class WeaponAttributesManager
{
private:
    std::unordered_map<WeaponType, WeaponAttributes> weaponAttributes;

private:
    WeaponAttributesManager();
    ~WeaponAttributesManager();

public:
    static WeaponAttributesManager &getInstance()
    {
        static WeaponAttributesManager instance;
        return instance;
    }

    const WeaponAttributes &getAttributes(WeaponType weapon) const;
};

#endif // WEAPONATTRIBUTESMANAGER_H
