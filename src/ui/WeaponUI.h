#ifndef WEAPONUI_H
#define WEAPONUI_H

#include <SFML/Graphics.hpp>

#include <unordered_map>

#include "constants/UpgradeBoxType.h"
#include "constants/WeaponType.h"

class WeaponUI
{
private:
    const std::unordered_map<UpgradeBoxType, sf::IntRect> &textureRects;

    sf::Sprite weaponSprite;
    sf::VertexArray cooldownOutline;

    bool uiVisible;

public:
    WeaponUI(const sf::Texture &texture,
             const std::unordered_map<UpgradeBoxType, sf::IntRect> &textureRects);
    ~WeaponUI();

    void update(WeaponType weaponType, float remainingTimeout);
    void render(sf::RenderWindow &window);
};

#endif // WEAPONUI_H
