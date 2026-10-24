
#include "drone_utils.h"

// Raylib imports
#include "raylib.h"
#include "raymath.h"

// C++ imports
#include "vector"

void updateDrone(Drone& drone, float dt) {
    drone.rotorShafts[0].rotation.y -= 1500.0f * dt;
}