#ifndef DRONE_UTILS_H
#define DRONE_UTILS_H

#include "raylib.h"

// Custom structs for Drone
typedef struct DronePart {
    Vector3 position; // Position relative to parent position
    Vector3 rotation; // Pitch, Yaw, Roll
    Vector3 scale = Vector3{ 1.0f, 1.0f, 1.0f };

    Color color;
} DronePart;

typedef struct Drone {
    Vector3 position; // World position of drone
    Vector3 rotation; // World rotation of drone

    float currentRotorSpeed = 0.0f;

    DronePart chassis;
    DronePart housing;
    DronePart arms[4];
    DronePart motorHousings[4];
    DronePart rotorShafts[4];
    DronePart propLefts[4];
    DronePart propRights[4];

} Drone;

#endif