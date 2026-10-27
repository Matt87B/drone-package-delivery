#include "drone_utils.h"

void ToggleActive(Drone& drone) {
    drone.isActive = !drone.isActive;
}