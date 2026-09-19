#pragma once

/**
 * @file radar.hpp
 * @brief Math utilities for 2D Radar projection, WorldToScreen, and FOV arrows.
 */

#include "world.hpp"
#include <cmath>
#include <numbers>

namespace math {

    inline bool WorldToScreen(const world::Vec3& pos, const float* view, int width, int height, float& outX, float& outY) {
        float w = view[3] * pos.x + view[7] * pos.y + view[11] * pos.z + view[15];
        if (w < 0.01f) return false;

        float x = view[0] * pos.x + view[4] * pos.y + view[8] * pos.z + view[12];
        float y = view[1] * pos.x + view[5] * pos.y + view[9] * pos.z + view[13];

        outX = (width / 2.0f) * (1.0f + x / w);
        outY = (height / 2.0f) * (1.0f - y / w);
        return true;
    }

    inline void RotatePoint(float x, float y, float angle, float& outX, float& outY) {
        float s = std::sin(angle);
        float c = std::cos(angle);
        outX = x * c - y * s;
        outY = x * s + y * c;
    }

    inline bool WorldToRadar(const world::Vec3& localPos, const world::Vec3& enemyPos, float viewYaw, float radarSize, float radarScale, float& outX, float& outY) {
        float dx = enemyPos.x - localPos.x;
        float dz = enemyPos.z - localPos.z;

        // Rotate relative to camera yaw
        float rotX, rotY;
        RotatePoint(dx, dz, -viewYaw, rotX, rotY);

        outX = rotX * radarScale;
        outY = rotY * radarScale;

        // Clamp to radar radius
        float radius = radarSize / 2.0f;
        float dist = std::sqrt(outX * outX + outY * outY);
        if (dist > radius) {
            outX = (outX / dist) * radius;
            outY = (outY / dist) * radius;
            return false; // Clamped (outside radar)
        }
        return true;
    }

    // Calculate an arrow pointing to an enemy outside the screen bounds
    inline void CalculateOOFArrow(const world::Vec3& localPos, const world::Vec3& enemyPos, float viewYaw, float screenW, float screenH, float offsetRadius, float& outX, float& outY, float& angle) {
        float dx = enemyPos.x - localPos.x;
        float dz = enemyPos.z - localPos.z;

        float rotX, rotY;
        RotatePoint(dx, dz, -viewYaw, rotX, rotY);

        angle = std::atan2(rotY, rotX);

        float centerX = screenW / 2.0f;
        float centerY = screenH / 2.0f;

        outX = centerX + std::cos(angle) * offsetRadius;
        outY = centerY + std::sin(angle) * offsetRadius;
    }
}
