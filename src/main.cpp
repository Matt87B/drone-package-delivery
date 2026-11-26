// C++ imports
#include "vector"

// Raylib imports
#include "raylib.h"
#include "raymath.h"

// Workspace imports
#include "mesh_utils.h"
#include "mat_utils.h"
#include "drone_utils.h"
#include "cam_utils.h"
#include "collision_manager.h"
#include "resources.h"

#define VIEWPORT_WIDTH 1920
#define VIEWPORT_HEIGHT 1080

#define ROTOR_SHAFT_SPEED 1500.0f

#define CAM_SMOOTH_SPEED 8.0f


// Lighting uniforms
Vector3 lightPos = { 20.0f, 20.0f, 20.0f };

float ambientStrength   = 0.4f;
float specularStrength  = 0.50f;
float shininess         = 200.0f;

// Main loop
int main(void) {

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "raylib - drone model");

    // Objects
    Camera3D camera         = CamUtils::Init();
    Camera3D orthoCamera    = CamUtils::InitOrtho();
    Drone drone             = DroneUtils::Init();

    SetTargetFPS(120);

    // Meshes
    Mesh housingMesh        = CreateCustomMesh(housingVertices, housingNormals, housingIndices);
    Mesh chassisMesh        = CreateCustomMesh(chassisVertices, chassisNormals, chassisIndices);
    Mesh armMesh            = CreateCustomMesh(armVertices, armNormals, armIndices);
    Mesh motorHousingMesh   = GenMeshCylinder(0.25f, 0.4f, 16);
    Mesh landingGearMesh    = CreateCustomMesh(landingGearVertices, landingGearNormals, landingGearIndices);
    Mesh rotorShaftMesh     = GenMeshCube(0.2f, 0.4f, 0.2f);
    Mesh propRightMesh      = CreateCustomMesh(propRightVertices, propRightNormals, propRightIndices);
    Mesh propLeftMesh       = CreateCustomMesh(propLeftVertices, propLeftNormals, propLeftIndices);

    // Models
    Model housingModel      = LoadModelFromMesh(housingMesh);
    Model chassisModel      = LoadModelFromMesh(chassisMesh);
    Model landingGearModel  = LoadModelFromMesh(landingGearMesh);
    Model armModel          = LoadModelFromMesh(armMesh);
    Model motorHousingModel = LoadModelFromMesh(motorHousingMesh);
    Model rotorShaftModel   = LoadModelFromMesh(rotorShaftMesh);
    Model propRightModel    = LoadModelFromMesh(propRightMesh);
    Model propLeftModel     = LoadModelFromMesh(propLeftMesh);

    // Shader
    Shader lighting = LoadShaderFromMemory(lightingVs, lightingFs);

    housingModel.materials[0].shader        = lighting;
    chassisModel.materials[0].shader        = lighting;
    landingGearModel.materials[0].shader    = lighting;
    armModel.materials[0].shader            = lighting;
    motorHousingModel.materials[0].shader   = lighting;
    rotorShaftModel.materials[0].shader     = lighting;
    propRightModel.materials[0].shader      = lighting;
    propLeftModel.materials[0].shader       = lighting;

    int baseColorLoc = GetShaderLocation(lighting, "baseColor");

    // Lighting

    int lightPosLoc     = GetShaderLocation(lighting, "lightPos");
    int viewPosLoc      = GetShaderLocation(lighting, "viewPos");
    int ambientLoc      = GetShaderLocation(lighting, "ambientStrength");
    int specularLoc     = GetShaderLocation(lighting, "specularStrength");
    int shininessLoc    = GetShaderLocation(lighting, "shininess");

    // Game loop
    while (!WindowShouldClose()) {
        
        float dt = GetFrameTime();
        float forwardInput  = 0.0f;
        float rightInput    = 0.0f;
        float throttleInput = 0.0f;
        float yawInput      = 0.0f;

        // Input polling
        if (IsKeyDown(KEY_W)) forwardInput  += 1.0f;
        if (IsKeyDown(KEY_S)) forwardInput  -= 1.0f;
        if (IsKeyDown(KEY_D)) rightInput    += 1.0f;
        if (IsKeyDown(KEY_A)) rightInput    -= 1.0f;
        if (IsKeyDown(KEY_R)) throttleInput += 1.0f;
        if (IsKeyDown(KEY_F)) throttleInput -= 1.0f;
        if (IsKeyDown(KEY_Q)) yawInput      += 1.0f;
        if (IsKeyDown(KEY_E)) yawInput      -= 1.0f;

        // Update Drone
        DroneUtils::UpdateThrottle(drone, throttleInput, dt);
        DroneUtils::UpdateRotation(drone, forwardInput, yawInput, rightInput, dt);
        DroneUtils::UpdateMotorMixer(drone, forwardInput, yawInput, rightInput);
        DroneUtils::UpdatePropellers(drone, dt);
        DroneUtils::UpdateKinematics(drone, dt);

        // Camera follow logic
        CamUtils::UpdateCamera(camera, drone, dt);
        CamUtils::UpdateOrthoCamera(orthoCamera, drone, dt);
        

        // Toggle drone active
        if (IsKeyPressed(KEY_SPACE)) DroneUtils::ToggleActive(drone);

        if (IsKeyPressed(KEY_HOME)) drone = DroneUtils::Init();

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
                    drone.orientation,
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
        
        Matrix landingGearWorld =
            ParentChild(
                housingWorld,
                TRS(
                    drone.landingGear.position,
                    drone.landingGear.rotation,
                    drone.landingGear.scale
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
                housingWorld,
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

        float feedSize = 512.0f;
        RenderTexture2D orthoFeed = LoadRenderTexture(feedSize, feedSize);
        
        BeginTextureMode(orthoFeed);
            ClearBackground(DARKGRAY);

            BeginMode3D(orthoCamera);
                DrawGrid(100, 1.0f);
            EndMode3D();
        EndTextureMode();

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                // Debugging for drone local orientation
                DrawAxes(3.0f, drone.position, drone.chassis.rotation);

                // Draw all 3D geometry
                DrawModelWithWorldMatrix(housingModel, housingWorld, lighting, housingColor);
                DrawModelWithWorldMatrix(chassisModel, chassisWorld, lighting, chassisColor);
                DrawModelWithWorldMatrix(landingGearModel, landingGearWorld, lighting, landingGearColor);
                
                for (Matrix armWorld : armWorlds) DrawModelWithWorldMatrix(armModel, armWorld, lighting, armColor);
                for (Matrix motorHousingWorld : motorHousingWorlds) DrawModelWithWorldMatrix(motorHousingModel, motorHousingWorld, lighting, motorHousingColor);
                for (Matrix rotorShaftWorld: rotorShaftWorlds) DrawModelWithWorldMatrix(rotorShaftModel, rotorShaftWorld, lighting, rotorShaftColor);
                for (Matrix propRightWorld: propRightWorlds) DrawModelWithWorldMatrix(propRightModel, propRightWorld, lighting, propColor);
                for (Matrix propLeftWorld: propLeftWorlds) DrawModelWithWorldMatrix(propLeftModel, propLeftWorld, lighting, propColor);

                DrawGrid(100, 1.0f);
                DrawSphere(lightPos, 0.5f, YELLOW);
                
            EndMode3D();

            float padding = 20.0f;
            Vector2 screenPos = {
                (float)GetScreenWidth() - feedSize - padding,
                (float)GetScreenHeight() - feedSize - padding
            };
            Rectangle sourceRec = { 0.0f, 0.0f, (float)orthoFeed.texture.width, (float)-orthoFeed.texture.height };
            DrawTextureRec(orthoFeed.texture, sourceRec, screenPos, WHITE);

            DrawRectangleLines((int)screenPos.x, (int)screenPos.y, feedSize, feedSize, GREEN);
            DrawLine((int)screenPos.x + feedSize/2 - 10, (int)screenPos.y + feedSize/2,
                    (int)screenPos.x + feedSize/2 + 10, (int)screenPos.y + feedSize/2, GREEN);
            DrawLine((int)screenPos.x + feedSize/2, (int)screenPos.y + feedSize/2 - 10,
                    (int)screenPos.x + feedSize/2, (int)screenPos.y + feedSize/2 + 10, GREEN);

            // Draw all 2D geometry here
            DrawFPS(10, 40);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
