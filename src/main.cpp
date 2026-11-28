// C++ imports
#include "vector"

// Raylib imports
#include "raylib.h"
#include "raymath.h"

// Workspace imports
#include "mesh_utils.h"
#include "mat_utils.h"
#include "drone.h"
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

// Main
int main(void) {

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "raylib - drone model");

    // Objects
    Camera3D camera         = CamUtils::Init();
    Camera3D orthoCamera    = CamUtils::InitOrtho();

    Drone drone(Vector3{ 0.0f, 5.0f, 0.0f });

    CollisionManager collisionManager(2.0f, Vector3{ 1.0f, 1.0f, 1.0f }); // <-------- FIX THIS TO BE A BOX
    
    std::vector<BoundingBox> worldObstacles = {
        BoundingBox{Vector3{ 4.0f, 0.0f, 4.0f }, Vector3{ 7.0f, 8.0f, 7.0f }},
        BoundingBox{Vector3{ -8.0f, 0.0f, -4.0f }, Vector3{ -5.0f, 6.0f, -1.0f }}
    };

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

    DroneModels droneModels = {
        housingModel,
        chassisModel,
        landingGearModel,
        armModel,
        motorHousingModel,
        rotorShaftModel,
        propRightModel,
        propLeftModel
    };

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

    float feedWidth = VIEWPORT_WIDTH/6;
    float feedHeight = VIEWPORT_HEIGHT/4;
    RenderTexture2D orthoFeed = LoadRenderTexture(feedWidth, feedHeight);

    // Game loop
    while (!WindowShouldClose()) {
        
        float dt = GetFrameTime();
        float pitchInput  = 0.0f;
        float rollInput    = 0.0f;
        float throttleInput = 0.0f;
        float yawInput      = 0.0f;

        // Input polling
        if (IsKeyDown(KEY_W)) pitchInput    += 1.0f;
        if (IsKeyDown(KEY_S)) pitchInput    -= 1.0f;
        if (IsKeyDown(KEY_D)) rollInput     += 1.0f;
        if (IsKeyDown(KEY_A)) rollInput     -= 1.0f;
        if (IsKeyDown(KEY_R)) throttleInput += 1.0f;
        if (IsKeyDown(KEY_F)) throttleInput -= 1.0f;
        if (IsKeyDown(KEY_Q)) yawInput      += 1.0f;
        if (IsKeyDown(KEY_E)) yawInput      -= 1.0f;

        // Update Drone
        drone.Update(pitchInput, yawInput, rollInput, throttleInput, dt);

        // Collision Logic
        collisionManager.HandleGroundCollision(drone.GetPositionRef(), drone.GetVelocityRef());
        collisionManager.HandleWorldObstacles(drone.GetPositionRef(), drone.GetVelocityRef(), worldObstacles);
        float altimeter = collisionManager.GetDistanceToGround(drone.GetPosition(), worldObstacles);

        // Camera follow logic
        CamUtils::UpdateCamera(camera, drone, dt);
        CamUtils::UpdateOrthoCamera(orthoCamera, drone, dt);
        
        // Toggle drone active
        if (IsKeyPressed(KEY_SPACE)) drone.ToggleActive();

        if (IsKeyPressed(KEY_HOME)) drone = Drone(Vector3{ 0.0f, 5.0f, 0.0f });

        // Init shader uniforms
        SetShaderValue(lighting, lightPosLoc, &lightPos, SHADER_UNIFORM_VEC3);
        SetShaderValue(lighting, viewPosLoc, &camera.position, SHADER_UNIFORM_VEC3);
        SetShaderValue(lighting, ambientLoc, &ambientStrength, SHADER_UNIFORM_FLOAT);
        SetShaderValue(lighting, specularLoc, &specularStrength, SHADER_UNIFORM_FLOAT);
        SetShaderValue(lighting, shininessLoc, &shininess, SHADER_UNIFORM_FLOAT);

        
        
        BeginTextureMode(orthoFeed);
            ClearBackground(DARKGRAY);

            BeginMode3D(orthoCamera);
                DrawGrid(100, 1.0f);
            EndMode3D();
        EndTextureMode();

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);

                // Draw all 3D geometry
                drone.Draw(droneModels, lighting);
                DrawGrid(100, 1.0f);
                DrawBoundingBox(collisionManager.GetAABB(drone.GetPositionRef()), GREEN); // <------------------- FIX THIS DRONE BOX (Check collision horizontal)

                for (const auto& box : worldObstacles) { DrawBoundingBox(box, RED); }
                
                DrawSphere(lightPos, 0.5f, YELLOW);
                
            EndMode3D();

            DrawText(TextFormat("Altimeter: %.2f m", altimeter), 10, 10, 20, DARKGRAY);

            float padding = 20.0f;
            Vector2 screenPos = {
                (float)GetScreenWidth() - feedWidth - padding,
                (float)GetScreenHeight() - feedHeight - padding
            };
            Rectangle sourceRec = { 0.0f, 0.0f, (float)orthoFeed.texture.width, (float)-orthoFeed.texture.height };
            DrawTextureRec(orthoFeed.texture, sourceRec, screenPos, WHITE);

            DrawRectangleLines((int)screenPos.x, (int)screenPos.y, feedWidth, feedHeight, GREEN);
            DrawLine((int)screenPos.x + feedWidth/2 - 10, (int)screenPos.y + feedHeight/2,
                    (int)screenPos.x + feedWidth/2 + 10, (int)screenPos.y + feedHeight/2, GREEN);
            DrawLine((int)screenPos.x + feedWidth/2, (int)screenPos.y + feedHeight/2 - 10,
                    (int)screenPos.x + feedWidth/2, (int)screenPos.y + feedHeight/2 + 10, GREEN);

            // Draw all 2D geometry here
            DrawFPS(10, 40);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
