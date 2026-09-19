#pragma once

/**
 * @file input.hpp
 * @brief Abstracted mouse input layer for toggling between standard API and hardware driver simulation.
 */

#include <Windows.h>

namespace input {

    enum Route {
        Standard = 0,
        HardwareDriver = 1
    };

    inline Route CurrentRoute = Standard;

    // Move the mouse relatively
    inline void MoveMouse(int dx, int dy) {
        if (dx == 0 && dy == 0) return;

        if (CurrentRoute == Standard) {
            INPUT input = { 0 };
            input.type = INPUT_MOUSE;
            input.mi.dx = dx;
            input.mi.dy = dy;
            input.mi.dwFlags = MOUSEEVENTF_MOVE;
            SendInput(1, &input, sizeof(INPUT));
        } else {
            // Placeholder for Arduino COM Port writing or G-Hub IOCTL
            // e.g. WriteFile(ArduinoHandle, buffer, size, nullptr, nullptr);
            // Fallback for now to standard so it doesn't break
            INPUT input = { 0 };
            input.type = INPUT_MOUSE;
            input.mi.dx = dx;
            input.mi.dy = dy;
            input.mi.dwFlags = MOUSEEVENTF_MOVE;
            SendInput(1, &input, sizeof(INPUT));
        }
    }

    // Click left mouse button
    inline void ClickMouse() {
        if (CurrentRoute == Standard) {
            INPUT input = { 0 };
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            SendInput(1, &input, sizeof(INPUT));
            
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &input, sizeof(INPUT));
        } else {
            // Hardware route placeholder
        }
    }
}
