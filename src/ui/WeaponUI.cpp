#include <cmath>

#include "WeaponUI.h"
#include "components/WeaponAttributesManager.h"
#include "constants/WeaponType.h"
#include "utils/GameUtils.h"

WeaponUI::WeaponUI(const sf::Texture &texture,
                   const std::unordered_map<UpgradeBoxType, sf::IntRect> &textureRects)
    : textureRects(textureRects),
      uiVisible(false)
{
    weaponSprite.setTexture(texture);
    weaponSprite.setScale(0.5f, 0.5f);

    cooldownOutline.setPrimitiveType(sf::Quads);
    cooldownOutline.resize(5); // Four edges + looping back to the start
}

WeaponUI::~WeaponUI()
{
    // Destructor
}

// Publics

void WeaponUI::update(WeaponType weaponType, float remainingTimeout)
{
    uiVisible = (weaponType != WeaponType::BASIC);

    if (uiVisible)
    {
        // Update rect for current weapon type
        UpgradeBoxType boxType = GameUtils::getUpgradeBoxTypeForWeaponType(weaponType);
        weaponSprite.setTextureRect(textureRects.at(boxType));

        // Handle "cooldown" progress
        float totalTimeout = WeaponAttributesManager::getInstance().getTimeout(weaponType);
        float cooldownPercentage = std::clamp(remainingTimeout / totalTimeout, 0.0f, 1.0f);

        sf::FloatRect spriteBounds = weaponSprite.getGlobalBounds();
        float left = spriteBounds.left;
        float top = spriteBounds.top;
        float right = left + spriteBounds.width;
        float bottom = top + spriteBounds.height;

        // Define points along the full path (counter-clockwise)
        std::vector<sf::Vector2f> fullPath = {
            {left, top}, {left, bottom}, {right, bottom}, {right, top}, {left, top}};

        // Total length of path for smooth interpolation
        float totalLength = spriteBounds.width * 2 + spriteBounds.height * 2;
        float currentProgress = cooldownPercentage * totalLength; // Map progress to the total outline length

        // Include vertexes as needed
        cooldownOutline.clear();

        float thickness = 1.0f;
        float traveled = 0.0f;

        for (size_t i = 0; i < fullPath.size() - 1; ++i)
        {
            float segmentLength = std::hypot(fullPath[i + 1].x - fullPath[i].x,
                                             fullPath[i + 1].y - fullPath[i].y);

            if (traveled + segmentLength <= currentProgress)
            {
                // Compute perpendicular offset for thickness
                sf::Vector2f direction = {fullPath[i + 1].x - fullPath[i].x,
                                          fullPath[i + 1].y - fullPath[i].y};

                float length = std::hypot(direction.x, direction.y);
                sf::Vector2f normal = {-direction.y / length, direction.x / length};

                sf::Vector2f p1 = fullPath[i] + normal * thickness;
                sf::Vector2f p2 = fullPath[i] - normal * thickness;
                sf::Vector2f p3 = fullPath[i + 1] - normal * thickness;
                sf::Vector2f p4 = fullPath[i + 1] + normal * thickness;

                // Append quad
                cooldownOutline.append(sf::Vertex(p1, sf::Color::Red));
                cooldownOutline.append(sf::Vertex(p2, sf::Color::Red));
                cooldownOutline.append(sf::Vertex(p3, sf::Color::Red));
                cooldownOutline.append(sf::Vertex(p4, sf::Color::Red));
            }
            else
            {
                // Partially complete the last segment
                float ratio = (currentProgress - traveled) / segmentLength;
                sf::Vector2f interpolatedPoint = {
                    fullPath[i].x + ratio * (fullPath[i + 1].x - fullPath[i].x),
                    fullPath[i].y + ratio * (fullPath[i + 1].y - fullPath[i].y)};

                sf::Vector2f direction = {interpolatedPoint.x - fullPath[i].x,
                                          interpolatedPoint.y - fullPath[i].y};

                float length = std::hypot(direction.x, direction.y);
                sf::Vector2f normal = {-direction.y / length, direction.x / length};

                sf::Vector2f p1 = fullPath[i] + normal * thickness;
                sf::Vector2f p2 = fullPath[i] - normal * thickness;
                sf::Vector2f p3 = interpolatedPoint - normal * thickness;
                sf::Vector2f p4 = interpolatedPoint + normal * thickness;

                cooldownOutline.append(sf::Vertex(p1, sf::Color::Red));
                cooldownOutline.append(sf::Vertex(p2, sf::Color::Red));
                cooldownOutline.append(sf::Vertex(p3, sf::Color::Red));
                cooldownOutline.append(sf::Vertex(p4, sf::Color::Red));
                break;
            }

            traveled += segmentLength;
        }
    }
}

void WeaponUI::render(sf::RenderWindow &window)
{
    if (uiVisible)
    {
        sf::View uiView = window.getDefaultView();
        sf::Vector2f uiViewCenter = uiView.getCenter();
        sf::Vector2f uiViewSize = uiView.getSize();

        float xPos = uiViewCenter.x - uiViewSize.x / 2.0f + 10.0f;
        float yPos = uiViewCenter.y - uiViewSize.y / 2.0f + 10.0f + 32.0f; // spacing + healthbar height
        weaponSprite.setPosition(xPos, yPos);

        window.draw(weaponSprite);
        window.draw(cooldownOutline);
    }
}