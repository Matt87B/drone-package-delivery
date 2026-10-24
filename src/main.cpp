// C++ imports
#include "vector"

// Raylib imports
#include "raylib.h"
#include "raymath.h"

// Workspace imports
#include "mesh_utils.h"
#include "mat_utils.h"
#include "resources.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define DRONE_SPEED 10.0f

// TODO: add physics based drone movement with thrust
//const Vector3 GRAVITY = Vector3{ 0.0f, -9.81f, 0.0f };
//const float MASS = 1.5f;
//const float INERTIA = Vector3{ 0.015f, 0.015f, 0.015f };

// Model texcoords and indices

std::vector<unsigned short> housingIndices = GenerateHardSurfaceIndices(housingVertices.size());
std::vector<Vector3> housingNormals = CalculateNormals(housingVertices, housingIndices);

std::vector<unsigned short> armIndices = GenerateHardSurfaceIndices(armVertices.size());
std::vector<Vector3> armNormals = CalculateNormals(armVertices, armIndices);

std::vector<unsigned short> propRightIndices = GenerateHardSurfaceIndices(propRightVertices.size());
std::vector<Vector3> propRightNormals = CalculateNormals(propRightVertices, propRightIndices);

std::vector<unsigned short> propLeftIndices = GenerateHardSurfaceIndices(propLeftVertices.size());
std::vector<Vector3> propLeftNormals = CalculateNormals(propLeftVertices, propLeftIndices);

// Drone Parameters

// Angles
Vector3 droneRotation = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 housingRotation = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 chassisRotation = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 armRotations[] = {
    Vector3{ 90.0f, 90.0f, 0.0f },
    Vector3{ 90.0f, 90.0f, 0.0f },
    Vector3{ -90.0f, -90.0f, 0.0f },
    Vector3{ -90.0f, -90.0f, 0.0f }
};
Vector3 motorHousingRotations[] = {
    Vector3{ 0.0f, 0.0f, 90.0f },
    Vector3{ 0.0f, 0.0f, 90.0f },
    Vector3{ 0.0f, 0.0f, 90.0f },
    Vector3{ 0.0f, 0.0f, 90.0f }
};
Vector3 rotorShaftRotations[] = {
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f }
};
Vector3 propRightRotations[] = {
    Vector3{ 90.0f, 0.0f, 0.0f },
    Vector3{ 90.0f, 0.0f, 0.0f },
    Vector3{ 90.0f, 0.0f, 180.0f },
    Vector3{ 90.0f, 0.0f, 180.0f }
};
Vector3 propLeftRotations[] = {
    Vector3{ 90.0f, 0.0f, 0.0f },
    Vector3{ 90.0f, 0.0f, 0.0f },
    Vector3{ 90.0f, 0.0f, 180.0f },
    Vector3{ 90.0f, 0.0f, 180.0f }
};

// Positions
Vector3 dronePosition   = Vector3{ 0.0f, 3.0f, 0.0f };
Vector3 housingPosition = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 chassisPosition = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 armPositions[]  = {
    Vector3{ 1.0f, 0.0f, 1.5f },
    Vector3{ 1.0f, 0.0f, -1.5f },
    Vector3{ -1.0f, 0.0f, 1.5f },
    Vector3{ -1.0f, 0.0f, -1.5f }
};
Vector3 motorHousingPositions[] = {
    Vector3{ 0.75f, 0.0f, 3.0f },
    Vector3{ 0.75f, 0.0f, 3.0f },
    Vector3{ 0.75f, 0.0f, 3.0f },
    Vector3{ 0.75f, 0.0f, 3.0f }
};
Vector3 rotorShaftPositions[] = {
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f }
};
Vector3 propRightPositions[] = {
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f }
};
Vector3 propLeftPositions[] = {
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f }
};

// Scales
Vector3 housingScale    = Vector3{ 1.0f, 1.0f, 1.0f };
Vector3 chassisScale    = Vector3{ 1.0f, 1.0f, 1.0f };
Vector3 armScale        = Vector3{ 1.0f, 1.0f, 1.0f };
Vector3 motorHousingScale = Vector3{ 0.5f, 0.5f, 0.5f };
Vector3 rotorShaftScale = Vector3{ 1.0f, 1.0f, 1.0f };
Vector3 propScale       = Vector3{ 0.9f, 0.9f, 0.9f };


// Lighting uniforms
Vector3 lightPos = { 20.0f, 20.0f, 20.0f };

float ambientStrength = 0.4f;
float specularStrength = 0.50f;
float shininess = 200.0f;

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

    DronePart chassis;
    DronePart housing;
    DronePart arms[4];
    DronePart motorHousings[4];
    DronePart rotorShafts[4];
    DronePart propLefts[4];
    DronePart propRights[4];

} Drone;

static void updateDrone(Drone& drone, float dt) {
    drone.rotorShafts[0].rotation.y -= 1500.0f * dt;
}

// Init functions

// Initialize Drone struct
static Drone InitDrone() {
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

// Initialize Camera3D
Camera3D InitCamera() {
    Camera3D camera = { 0 };
    camera.position = (Vector3) { 0.0f, 10.0f, 10.0f };
    camera.target = (Vector3) { 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3) { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    return camera;
}

static void DrawAxes(float len) {
    DrawLine3D({0, 0, 0}, {len, 0, 0}, RED);
    DrawLine3D({0, 0, 0}, {0, len, 0}, GREEN);
    DrawLine3D({0, 0, 0}, {0, 0, len}, BLUE);
    DrawSphere({0, 0, 0}, 0.09f, YELLOW);
}

// Main loop
int main(void) {

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(800, 450, "raylib - drone model");

    // Objects
    Camera3D camera = InitCamera();
    Drone drone = InitDrone();

    SetTargetFPS(60);

    // Meshes
    Mesh housingMesh   = CreateCustomMesh(housingVertices, housingNormals, housingIndices);
    Mesh chassisMesh   = GenMeshCube(0.75f, 0.75f, 0.5f);
    Mesh armMesh       = CreateCustomMesh(armVertices, armNormals, armIndices);
    Mesh motorHousingMesh = GenMeshCylinder(0.25f, 0.4f, 16);
    Mesh rotorShaftMesh      = GenMeshCube(0.2f, 0.4f, 0.2f);
    Mesh propRightMesh = CreateCustomMesh(propRightVertices, propRightNormals, propRightIndices);
    Mesh propLeftMesh = CreateCustomMesh(propLeftVertices, propLeftNormals, propLeftIndices);

    // Models
    Model housingModel = LoadModelFromMesh(housingMesh);
    Model chassisModel = LoadModelFromMesh(chassisMesh);
    Model armModel     = LoadModelFromMesh(armMesh);
    Model motorHousingModel = LoadModelFromMesh(motorHousingMesh);
    Model rotorShaftModel    = LoadModelFromMesh(rotorShaftMesh);
    Model propRightModel = LoadModelFromMesh(propRightMesh);
    Model propLeftModel = LoadModelFromMesh(propLeftMesh);

    // Shader
    Shader lighting = LoadShaderFromMemory(lightingVs, lightingFs);

    housingModel.materials[0].shader   = lighting;
    chassisModel.materials[0].shader   = lighting;
    armModel.materials[0].shader       = lighting;
    motorHousingModel.materials[0].shader = lighting;
    rotorShaftModel.materials[0].shader   = lighting;
    propRightModel.materials[0].shader = lighting;
    propLeftModel.materials[0].shader = lighting;

    int baseColorLoc = GetShaderLocation(lighting, "baseColor");

    // Lighting

    int lightPosLoc = GetShaderLocation(lighting, "lightPos");
    int viewPosLoc = GetShaderLocation(lighting, "viewPos");
    int ambientLoc = GetShaderLocation(lighting, "ambientStrength");
    int specularLoc = GetShaderLocation(lighting, "specularStrength");
    int shininessLoc = GetShaderLocation(lighting, "shininess");

    // Game loop
    while (!WindowShouldClose()) {
        // Key listeners
        updateDrone(drone, GetFrameTime());

        float pitch = drone.rotation.x * DEG2RAD;
        float yaw = drone.rotation.y * DEG2RAD;
        float roll = drone.rotation.z * DEG2RAD;

        Vector3 forwardVec = GetForwardVector(yaw);
        Vector3 rightVec = GetRightVector(yaw);
        Vector3 upVec = GetUpVector(pitch, yaw);

        float moveDistance = DRONE_SPEED * GetFrameTime();

        if (IsKeyDown(KEY_W)) {
            Matrix transform = Translate(ScaleVector(forwardVec, moveDistance));
            drone.position = TransformPoint(drone.position, transform);
        }
        if (IsKeyDown(KEY_A)) {
            Matrix transform = Translate(ScaleVector(rightVec, moveDistance));
            drone.position = TransformPoint(drone.position, transform);
        }
        if (IsKeyDown(KEY_S)) {
            Matrix transform = Translate(ScaleVector(forwardVec, -moveDistance));
            drone.position = TransformPoint(drone.position, transform);
        }
        if (IsKeyDown(KEY_D)) {
            Matrix transform = Translate(ScaleVector(rightVec, -moveDistance));
            drone.position = TransformPoint(drone.position, transform);
        }
        if (IsKeyDown(KEY_R)) {
            Matrix transform = Translate(ScaleVector(upVec, moveDistance));
            drone.position = TransformPoint(drone.position, transform);
        }
        if (IsKeyDown(KEY_F) || IsKeyDown(KEY_LEFT_CONTROL)) {
            Matrix transform = Translate(ScaleVector(upVec, -moveDistance));
            drone.position = TransformPoint(drone.position, transform);
        }
        if (IsKeyDown(KEY_E)) {
            drone.rotation.y -= 45.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_Q)) {
            drone.rotation.y += 45.0f * GetFrameTime();
        }

        // Init shader uniforms

        SetShaderValue(lighting, lightPosLoc, &lightPos, SHADER_UNIFORM_VEC3);
        SetShaderValue(lighting, viewPosLoc, &camera.position, SHADER_UNIFORM_VEC3);
        SetShaderValue(lighting, ambientLoc, &ambientStrength, SHADER_UNIFORM_FLOAT);
        SetShaderValue(lighting, specularLoc, &specularStrength, SHADER_UNIFORM_FLOAT);
        SetShaderValue(lighting, shininessLoc, &shininess, SHADER_UNIFORM_FLOAT);


        // Hierarchical matrices

        // World root
        Matrix worldRoot = MatrixIdentity();

        // Child of world root
        Matrix droneWorld =
            ParentChild(
                worldRoot,
                TRS(
                    drone.position,
                    drone.rotation,
                    Vector3{ 1.0f, 1.0f, 1.0f } //Drone world's scale is always 1
                )
            );

        // Childs of drone world
        Matrix housingWorld =
            ParentChild(
                droneWorld,
                TRS(
                    drone.housing.position,
                    drone.housing.rotation,
                    drone.housing.scale
                )
            );

        Matrix chassisWorld =
            ParentChild(
                housingWorld,
                TRS(
                    drone.chassis.position,
                    drone.chassis.rotation,
                    drone.chassis.scale
                )
            );

        Matrix armWorlds[std::size(drone.arms)];
        for (int i = 0; i < std::size(drone.arms); i++) {
            armWorlds[i] = 
                ParentChild(
                chassisWorld,
                TRS(
                    drone.arms[i].position,
                    drone.arms[i].rotation,
                    drone.arms[i].scale
                )
            );
        }

        Matrix motorHousingWorlds[std::size(drone.motorHousings)];
        for (int i = 0; i < std::size(drone.motorHousings); i++) {
            motorHousingWorlds[i] = 
                ParentChild(
                armWorlds[i],
                TRS(
                    drone.motorHousings[i].position,
                    drone.motorHousings[i].rotation,
                    drone.motorHousings[i].scale
                )
            );
        }

        Matrix rotorShaftWorlds[std::size(drone.rotorShafts)];
        for (int i = 0; i < std::size(drone.rotorShafts); i++) {
            rotorShaftWorlds[i] =
                ParentChild(
                motorHousingWorlds[i],
                TRS(
                    drone.rotorShafts[i].position,
                    drone.rotorShafts[i].rotation,
                    drone.rotorShafts[i].scale
                )
            );
        }

        Matrix propRightWorlds[std::size(drone.propRights)];
        for (int i = 0; i < std::size(drone.propRights); i++) {
            propRightWorlds[i] = 
                ParentChild(
                rotorShaftWorlds[(i % 2) + 2],
                TRS(
                    drone.propRights[i].position,
                    drone.propRights[i].rotation,
                    drone.propRights[i].scale
                )
            );
        }

        Matrix propLeftWorlds[std::size(drone.propLefts)];
        for (int i = 0; i < std::size(drone.propLefts); i++) {
            propLeftWorlds[i] = 
                ParentChild(
                rotorShaftWorlds[i % 2],
                TRS(
                    drone.propLefts[i].position,
                    drone.propLefts[i].rotation,
                    drone.propLefts[i].scale
                )
            );
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                
                // Draw all 3D geometry
                DrawModelWithWorldMatrix(housingModel, housingWorld, lighting, housingColor);
                DrawModelWithWorldMatrix(chassisModel, chassisWorld, lighting, chassisColor);
                for (Matrix armWorld : armWorlds) {
                    DrawModelWithWorldMatrix(armModel, armWorld, lighting, armColor);
                }
                for (Matrix motorHousingWorld : motorHousingWorlds) {
                    DrawModelWithWorldMatrix(motorHousingModel, motorHousingWorld, lighting, motorHousingColor);
                }
                for (Matrix rotorShaftWorld: rotorShaftWorlds) {
                    DrawModelWithWorldMatrix(rotorShaftModel, rotorShaftWorld, lighting, rotorShaftColor);
                }
                for (Matrix propRightWorld: propRightWorlds) {
                    DrawModelWithWorldMatrix(propRightModel, propRightWorld, lighting, propColor);
                }
                for (Matrix propLeftWorld: propLeftWorlds) {
                    DrawModelWithWorldMatrix(propLeftModel, propLeftWorld, lighting, propColor);
                }

                DrawGrid(100, 1.0f);
                DrawAxes(3.0f);
                DrawSphere(lightPos, 0.5f, YELLOW);
                
            EndMode3D();

            // Draw all 2D geometry here
            DrawFPS(10, 40);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
