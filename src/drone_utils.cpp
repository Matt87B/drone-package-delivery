#include "drone_utils.h"
#include "resources.h"

void ToggleActive(Drone& drone) {
    drone.isActive = !drone.isActive;
}

// Initialize Drone struct
Drone InitDrone() {
    Drone drone = { 0 };
    drone.position = dronePosition;
    drone.rotation = droneRotation;
    
    drone.chassis.position = chassisPosition;
    drone.chassis.rotation = chassisRotation;
    drone.chassis.scale = chassisScale;
    drone.chassis.color = chassisColor;

    drone.housing.position = housingPosition;
    drone.housing.rotation = housingRotation;
    drone.housing.scale = housingScale;
    drone.housing.color = housingColor;

    drone.landingGear.position = landingGearPosition;
    drone.landingGear.rotation = landingGearRotation;
    drone.landingGear.scale = landingGearScale;
    drone.landingGear.color = landingGearColor;

    for (DronePart arm : drone.arms) {
        arm.color = armColor;
    }
    for (int i = 0; i < std::size(drone.arms); i++) {
        drone.arms[i].position = armPositions[i];
        drone.arms[i].rotation = armRotations[i];
        drone.arms[i].scale = armScale;
    }

    for (DronePart motorHousing : drone.motorHousings) {
        motorHousing.color = motorHousingColor;
    }
    for (int i = 0; i < std::size(drone.motorHousings); i++) {
        drone.motorHousings[i].position = motorHousingPositions[i];
        drone.motorHousings[i].rotation = motorHousingRotations[i];
        drone.motorHousings[i].scale = motorHousingScale;
    }

    for (DronePart rotorShaft : drone.rotorShafts) {
        rotorShaft.color = rotorShaftColor;
    }
    for (int i = 0; i < std::size(drone.rotorShafts); i++) {
        drone.rotorShafts[i].position = rotorShaftPositions[i];
        drone.rotorShafts[i].rotation = rotorShaftRotations[i];
        drone.rotorShafts[i].scale = rotorShaftScale;
    }

    for (DronePart propRight : drone.propRights) {
        propRight.color = propColor;
    }
    for (int i = 0; i < std::size(drone.propRights); i++) {
        drone.propRights[i].position = propRightPositions[i];
        drone.propRights[i].rotation = propRightRotations[i];
        drone.propRights[i].scale = propScale;
    }

    for (DronePart propLeft : drone.propLefts) {
        propLeft.color = propColor;
    }
    for (int i = 0; i < std::size(drone.propLefts); i++) {
        drone.propLefts[i].position = propLeftPositions[i];
        drone.propLefts[i].rotation = propLeftRotations[i];
        drone.propLefts[i].scale = propScale;
    }

    return drone;
}