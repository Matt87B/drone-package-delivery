#include "raylib.h"
#include "raymath.h"
#include "vector"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// Lighting and Shader definitions
static const char* lightingVs = R"(
#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

uniform mat4 mvp;
uniform mat4 matModel;

out vec3 fragPos;
out vec3 fragNormal;
out vec4 fragColor;

    void main() {
        vec4 worldPos = matModel * vec4(vertexPosition, 1.0);
        fragPos = worldPos.xyz;

        // Correct normal transform for non-uniform scaling
        fragNormal = mat3(transpose(inverse(matModel))) * vertexNormal;

        fragColor = vertexColor;
        gl_Position = mvp * vec4(vertexPosition, 1.0);
    }
)";

static const char* lightingFs = R"(
#version 330

in vec3 fragPos;
in vec3 fragNormal;
in vec4 fragColor;

out vec4 finalColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec4 baseColor;

uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;

    void main() {
        vec3 N = normalize(fragNormal);
        vec3 L = normalize(lightPos - fragPos);
        vec3 V = normalize(viewPos - fragPos);
        vec3 R = reflect(-L, N);

        // Ambient
        vec3 ambient = ambientStrength * baseColor.rgb;

        // Diffuse
        float diff = max(dot(N, L), 0.0);
        vec3 diffuse = diff * baseColor.rgb;

        // Specular
        float spec = 0.0;
        if (diff > 0.0)
        {
            spec = pow(max(dot(V, R), 0.0), shininess);
        }
        vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);

        vec3 result = ambient + diffuse + specular;

        finalColor = vec4(result, baseColor.a) * fragColor;
    }
)";

// Automatically calculates normals for meshes
std::vector<Vector3> CalculateNormals(const std::vector<Vector3>& vertices, const std::vector<unsigned short>& indices) {
    std::vector<Vector3> normals(vertices.size(), { 0.0f, 0.0f, 0.0f });

    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned short idx0 = indices[i];
        unsigned short idx1 = indices[i + 1];
        unsigned short idx2 = indices[i + 2];

        Vector3 v0 = vertices[idx0];
        Vector3 v1 = vertices[idx1];
        Vector3 v2 = vertices[idx2];

        Vector3 edge1 = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
        Vector3 edge2 = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };

        // Cross product
        Vector3 faceNormal = {
            edge1.y * edge2.z - edge1.z * edge2.y,
            edge1.z * edge2.x - edge1.x * edge2.z,
            edge1.x * edge2.y - edge1.y * edge2.x
        };

        normals[idx0].x += faceNormal.x; normals[idx0].y += faceNormal.y; normals[idx0].z += faceNormal.z;
        normals[idx1].x += faceNormal.x; normals[idx1].y += faceNormal.y; normals[idx1].z += faceNormal.z;
        normals[idx2].x += faceNormal.x; normals[idx2].y += faceNormal.y; normals[idx2].z += faceNormal.z;
    }

    for (auto& normal : normals) {
        float length = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        if (length > 0.0f) {
            normal.x /= length;
            normal.y /= length;
            normal.z /= length;
        }
    }

    return normals;
}

// Generate indices for unshared vertices
std::vector<unsigned short> GenerateSequentialIndices(size_t vertexCount) {
    std::vector<unsigned short> indices(vertexCount);
    for (size_t i = 0; i < vertexCount; i++) {
        indices[i] = static_cast<unsigned short>(i);
    }
    return indices;
}

// Model vertices, texcoords and indices

std::vector<Vector3> housingVertices = {
    //Front face
    { -1.0f, 0.5f, 1.5f }, { -1.5f, -0.5f, 1.5f }, { 1.0f, 0.5f, 1.5f },
    { 1.0f, 0.5f, 1.5f }, { -1.5f, -0.5f, 1.5f }, { 1.5f, -0.5f, 1.5f },

    // Right face
    { -1.0f, 0.5f, -1.5f }, { -1.5f, -0.5f, -1.5f }, { -1.0f, 0.5f, 1.5f },
    { -1.0f, 0.5f, 1.5f }, { -1.5f, -0.5f, -1.5f }, { -1.5f, -0.5f, 1.5f },

    // Back face
    { 1.0f, 0.5f, -1.5f }, { 1.5f, -0.5f, -1.5f }, { -1.0f, 0.5f, -1.5f },
    { -1.0f, 0.5f, -1.5f }, { 1.5f, -0.5f, -1.5f }, { -1.5f, -0.5f, -1.5f },

    // Left face
    { 1.0f, 0.5f, 1.5f }, { 1.5f, -0.5f, 1.5f }, { 1.0f, 0.5f, -1.5f },
    { 1.0f, 0.5f, -1.5f }, { 1.5f, -0.5f, 1.5f }, { 1.5f, -0.5f, -1.5f },

    // Top face
    { -1.0f, 0.5f, 1.5f }, { 1.0f, 0.5f, 1.5f }, { -1.0f, 0.5f, -1.5f },
    { -1.0f, 0.5f, -1.5f }, { 1.0f, 0.5f, 1.5f }, { 1.0f, 0.5f, -1.5f },

    // Bottom face
    { -1.5f, -0.5f, -1.5f }, { 1.5f, -0.5f, -1.5f }, { -1.5f, -0.5f, 1.5f },
    { -1.5f, -0.5f, 1.5f }, { 1.5f, -0.5f, -1.5f }, { 1.5f, -0.5f, 1.5f }
};

std::vector<Vector3> armVertices = {
    // Back cap
    { 0.0f, 0.25f, 0.0f }, { 0.5f, 0.0f, 0.0f }, { -0.5f, 0.0f, 0.0f },
    { 0.5f, 0.0f, 0.0f }, { 0.0f, -0.25f, 0.0f }, { -0.5f, 0.0f, 0.0f },

    // Arm

    { 0.0f, 0.25f, 0.0f }, { 0.0f, 0.25f, 1.5f }, { 0.5f, 0.0f, 0.0f },
    { 0.0f, 0.25f, 1.5f }, { 0.25f, 0.0f, 1.5f }, { 0.5f, 0.0f, 0.0f },

    { 0.0f, 0.25f, 0.0f }, { -0.5f, 0.0f, 0.0f }, { 0.0f, 0.25f, 1.5f },
    { 0.0f, 0.25f, 1.5f }, { -0.5f, 0.0f, 0.0f }, { -0.25f, 0.0f, 1.5f },

    { 0.5f, 0.0f, 0.0f }, { 0.25f, 0.0f, 1.5f }, { 0.0f, -0.25f, 0.0f },
    { 0.0f, -0.25f, 0.0f }, { 0.25f, 0.0f, 1.5f }, {0.0f, -0.25f, 1.5f },

    { -0.5f, 0.0f, 0.0f }, { 0.0f, -0.25f, 1.5f }, { -0.25f, 0.0f, 1.5f },
    { -0.5f, 0.0f, 0.0f }, { 0.0f, -0.25f, 0.0f }, { 0.0f, -0.25f, 1.5f },

    // Wrist

    { 1.0f, 0.25f, 4.0f }, { 0.0f, 0.25f, 1.5f }, { -0.25f, 0.0f, 1.5f },
    { 1.0f, 0.25f, 4.0f }, { -0.25f, 0.0f, 1.5f }, { 0.75f, 0.0f, 4.0f },

    { 0.0f, 0.25f, 1.5f }, { 1.0f, 0.25f, 4.0f },  { 0.25f, 0.0f, 1.5f },
    { 0.25f, 0.0f, 1.5f }, { 1.0f, 0.25f, 4.0f },  { 1.25f, 0.0f, 4.0f },

    { 1.25f, 0.0f, 4.0f }, { 0.0f, -0.25f, 1.5f }, { 0.25f, 0.0f, 1.5f },
    { 1.25f, 0.0f, 4.0f }, { 1.0f, -0.25f, 4.0f }, { 0.0f, -0.25f, 1.5f },

    { 1.0f, -0.25f, 4.0f }, { 0.75f, 0.0f, 4.0f }, { -0.25f, 0.0f, 1.5f },
    { 1.0f, -0.25f, 4.0f }, { -0.25f, 0.0f, 1.5f }, {0.0f, -0.25f, 1.5f },

    // Front rect

    { 1.0f, 0.25f, 4.5f }, { 1.25f, 0.0f, 4.5f }, { 1.0f, 0.25f, 4.0f },

    // Front cap
    
    { 0.75f, 0.0f, 4.5f }, { 1.25f, 0.0f, 4.5f }, { 1.0f, 0.25f, 4.5f }, 
    { 0.75f, 0.0f, 4.5f }, { 1.0f, -0.25f, 4.5f }, { 1.25f, 0.0f, 4.5f }
};

std::vector<unsigned short> housingIndices = GenerateSequentialIndices(housingVertices.size());
std::vector<Vector3> housingNormals = CalculateNormals(housingVertices, housingIndices);

std::vector<unsigned short> armIndices = GenerateSequentialIndices(armVertices.size());
std::vector<Vector3> armNormals = CalculateNormals(armVertices, armIndices);

// Drone Parameters

// Colours
static Color housingColor = Color{ 40, 120,  40, 255 };
static Color chassisColor = Color{ 128, 128, 128, 255 };
static Color armColor = Color{ 25, 31, 52, 255 };

// Angles
Vector3 droneRotation = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 housingRotation = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 chassisRotation = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 armRotations[] = {
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f }
};
// Positions
Vector3 dronePosition   = Vector3{ 0.0f, 3.0f, 0.0f };
Vector3 housingPosition = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 chassisPosition = Vector3{ 0.0f, 0.0f, 1.75f };
Vector3 armPositions[]  = {
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 0.0f }
};

// Scales
Vector3 housingScale    = Vector3{ 1.0f, 1.0f, 1.0f };
Vector3 chassisScale    = Vector3{ 1.0f, 1.0f, 1.0f };
Vector3 armScale        = Vector3{ 1.0f, 1.0f, 1.0f };

// Lighting uniforms
Vector3 lightPos = { 20.0f, 20.0f, 20.0f };

float ambientStrength = 0.3f;
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
} Drone;

// Matrix transformation helpers

static Matrix MatIdentity() {
    return MatrixIdentity();
}

static Matrix MatTranslation(float tx, float ty, float tz) {
    Matrix m = MatrixIdentity();
    m.m12 = tx;
    m.m13 = ty;
    m.m14 = tz;
    return m;
}

static Matrix MatScale(float sx, float sy, float sz) {
    Matrix m = MatrixIdentity();
    m.m0 = sx;
    m.m5 = sy;
    m.m10 = sz;
    return m;
}

static Matrix MatRotationX(float a) {
    float c = cosf(a), s = sinf(a);
    Matrix m = MatrixIdentity();
    m.m5 = c;
    m.m9 = -s;
    m.m6 = s;
    m.m10 = c;
    return m;
}

static Matrix MatRotationY(float a) {
    float c = cosf(a), s = sinf(a);
    Matrix m = MatrixIdentity();
    m.m0 = c;
    m.m8 = s;
    m.m2 = -s;
    m.m10 = c;
    return m;
}

static Matrix MatRotationZ(float a) {
    float c = cosf(a), s = sinf(a);
    Matrix m = MatrixIdentity();
    m.m0 = c;
    m.m4 = -s;
    m.m1 = s;
    m.m5 = c;
    return m;
}

static Matrix MatMul(Matrix A, Matrix B) {
    float a[4][4] = {{A.m0, A.m4, A.m8, A.m12},
                     {A.m1, A.m5, A.m9, A.m13},
                     {A.m2, A.m6, A.m10, A.m14},
                     {A.m3, A.m7, A.m11, A.m15}};
    float b[4][4] = {{B.m0, B.m4, B.m8, B.m12},
                     {B.m1, B.m5, B.m9, B.m13},
                     {B.m2, B.m6, B.m10, B.m14},
                     {B.m3, B.m7, B.m11, B.m15}};
    float c[4][4] = {};
    for (int r = 0; r < 4; ++r)
    {
        for (int col = 0; col < 4; ++col)
        {
            for (int k = 0; k < 4; ++k)
            {
                c[r][col] += a[r][k] * b[k][col];
            }
        }
    }

    Matrix M = {};
    M.m0 = c[0][0];
    M.m4 = c[0][1];
    M.m8 = c[0][2];
    M.m12 = c[0][3];
    M.m1 = c[1][0];
    M.m5 = c[1][1];
    M.m9 = c[1][2];
    M.m13 = c[1][3];
    M.m2 = c[2][0];
    M.m6 = c[2][1];
    M.m10 = c[2][2];
    M.m14 = c[2][3];
    M.m3 = c[3][0];
    M.m7 = c[3][1];
    M.m11 = c[3][2];
    M.m15 = c[3][3];
    return M;
}

// Rotate about all three axes
static Matrix MatRotation(float rx, float ry, float rz) {
    Matrix matX = MatRotationX(rx);
    Matrix matY = MatRotationY(ry);
    Matrix matZ = MatRotationZ(rz);

    return MatMul(MatMul(matX, matY), matZ);
}

// Wrappers for matrix operations
static Matrix Translate(Vector3 v) {
    return MatTranslation(v.x, v.y, v.z);
}

static Matrix Scale(Vector3 v) {
    return MatScale(v.x, v.y, v.z);
}

static Matrix Rotation(Vector3 v) {
    return MatRotation(v.x, v.y, v.z);
}

// Matrix translate rotate scale
static Matrix TRS(const Vector3& t, const Vector3& r, const Vector3& s) {
    Matrix T = Translate(t);
    Matrix R = Rotation(r);
    Matrix S = Scale(s);
    return MatrixMultiply(MatrixMultiply(S, R), T);
}

// Assign a child matrix to a world
static Matrix ParentChild(const Matrix& parentWorld, const Matrix& childLocal) {
    return MatrixMultiply(childLocal, parentWorld);
}

// Transform a vector in accordance with a given transformation matrix
static Vector3 TransformPoint(Vector3 p, Matrix m) {
    return Vector3Transform(p, m);
}

// Draw model relative to a world matrix
static void DrawModelWithWorldMatrix(Model& model, const Matrix& world, Shader shader, Color tint = WHITE) {
    Matrix prev = model.transform;
    model.transform = world;

    float color[4] =
    {
        tint.r / 255.0f,
        tint.g / 255.0f,
        tint.b / 255.0f,
        tint.a / 255.0f
    };

    int baseColorLoc = GetShaderLocation(shader, "baseColor");
    SetShaderValue(shader, baseColorLoc, color, SHADER_UNIFORM_VEC4);

    DrawModel(model, Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);

    model.transform = prev;
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
        arm.scale = armScale;
        arm.color = armColor;
    }
    for (int i = 0; i < 4; i++) {
        drone.arms[i].position = armPositions[i];
        drone.arms[i].rotation = armRotations[i];
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

// Draw and custom mesh functions

static void DrawAxes(float len) {
    DrawLine3D({0, 0, 0}, {len, 0, 0}, RED);
    DrawLine3D({0, 0, 0}, {0, len, 0}, GREEN);
    DrawLine3D({0, 0, 0}, {0, 0, len}, BLUE);
    DrawSphere({0, 0, 0}, 0.09f, YELLOW);
}

// Create a custom mesh
static Mesh CreateCustomMesh(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<unsigned short>& indices) {
    Mesh mesh = { 0 };

    mesh.vertexCount = vertices.size();
    mesh.triangleCount = indices.size() / 3;

    // Vertices
    if (mesh.vertexCount > 0) {
        mesh.vertices = (float*)MemAlloc(vertices.size() * 3 * sizeof(float));

        for (size_t i = 0; i < vertices.size(); i++) {
            mesh.vertices[i * 3]     = vertices[i].x;
            mesh.vertices[i * 3 + 1] = vertices[i].y;
            mesh.vertices[i * 3 + 2] = vertices[i].z;
        }
    }

    // Normals
    if (mesh.vertexCount > 0 && !normals.empty()) {
        mesh.normals = (float*)MemAlloc(normals.size() * 3 * sizeof(float));
        for (size_t i = 0; i < normals.size(); i++) {
            mesh.normals[i * 3]     = normals[i].x;
            mesh.normals[i * 3 + 1] = normals[i].y;
            mesh.normals[i * 3 + 2] = normals[i].z;
        }
    }

    // Indices for avoiding duplicate vertices
    if (mesh.triangleCount > 0) {
        mesh.indices = (unsigned short*)MemAlloc(indices.size() * sizeof(unsigned short));
        for (size_t i = 0; i < indices.size(); i++) {
            mesh.indices[i] = indices[i];
        }
    }

    mesh.texcoords = (float*)MemAlloc(mesh.vertexCount * 2 * sizeof(float));

    UploadMesh(&mesh, false);

    return mesh;
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
    Mesh droneHousingMesh   = CreateCustomMesh(housingVertices, housingNormals, housingIndices);
    Mesh droneChassisMesh   = GenMeshCube(0.75f, 0.75f, 0.5f);
    Mesh droneArmMesh       = CreateCustomMesh(armVertices, armNormals, armIndices);

    // Models
    Model droneHousingModel = LoadModelFromMesh(droneHousingMesh);
    Model droneChassisModel = LoadModelFromMesh(droneChassisMesh);
    Model droneArmModel     = LoadModelFromMesh(droneArmMesh);

    // Shader
    Shader lighting = LoadShaderFromMemory(lightingVs, lightingFs);

    droneHousingModel.materials[0].shader   = lighting;
    droneChassisModel.materials[0].shader   = lighting;
    droneArmModel.materials[0].shader       = lighting;

    int baseColorLoc = GetShaderLocation(lighting, "baseColor");

    // Lighting

    int lightPosLoc = GetShaderLocation(lighting, "lightPos");
    int viewPosLoc = GetShaderLocation(lighting, "viewPos");
    int ambientLoc = GetShaderLocation(lighting, "ambientStrength");
    int specularLoc = GetShaderLocation(lighting, "specularStrength");
    int shininessLoc = GetShaderLocation(lighting, "shininess");

    while (!WindowShouldClose()) {

        // Key listeners

        if (IsKeyDown(KEY_A)) {
            drone.position.x -= 5.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_D)) {
            drone.position.x += 5.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_S)) {
            drone.position.z -= 5.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_W)) {
            drone.position.z += 5.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_C) || IsKeyDown(KEY_LEFT_CONTROL)) {
            drone.position.y -= 5.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_SPACE)) {
            drone.position.y += 5.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_RIGHT)) {
            drone.rotation.y += 5.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_LEFT)) {
            drone.rotation.y -= 5.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_UP)) {
            drone.rotation.x += 5.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_DOWN)) {
            drone.rotation.x -= 5.0f * GetFrameTime();
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
                droneWorld,
                TRS(
                    drone.chassis.position,
                    drone.chassis.rotation,
                    drone.chassis.scale
                )
            );

        Matrix armWorlds[4];
        for (int i = 0; i < 4; i++) {
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

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                
                // Draw all 3D geometry here
                //DrawModelWithWorldMatrix(droneHousingModel, housingWorld, lighting, housingColor);
                //DrawModelWithWorldMatrix(droneChassisModel, chassisWorld, lighting, chassisColor);
                for (Matrix armWorld : armWorlds) {
                    DrawModelWithWorldMatrix(droneArmModel, armWorld, lighting, armColor);
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
