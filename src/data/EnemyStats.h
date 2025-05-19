#pragma once

struct EnemyStats
{
    int pointValue;
    float health;
    float speed;
    float fireRateCooldown;
    float activeDistance;

    static EnemyStats CreateEmpty()
    {
        return {0, 0.0f, 0.0f, 0.0f, 0.0f};
    }

    static EnemyStats CreateBasicStats(int pointsValue, float health, float speed)
    {
        return {pointsValue, health, speed, 0.0f, 0.0f};
    }

    static EnemyStats CreateRangedStats(int pointsValue, float health, float speed,
                                        float fireRateCooldown, float activeDistance)
    {
        return {pointsValue, health, speed, fireRateCooldown, activeDistance};
    }
};