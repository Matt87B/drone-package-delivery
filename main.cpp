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

// Model vertices, texcoords and indices

std::vector<Vector3> housingVertices = {
    // Top
    { -0.5f, 0.8f, -1.5f }, { -0.5f, 1.0f, 0.0f }, { 0.5f, 0.8f, -1.5f },
    { 0.5f, 0.8f, -1.5f }, { -0.5f, 1.0f, 0.0f }, { 0.5f, 1.0f, 0.0f },

    { 0.5f, 0.8f, -1.5f }, { 0.5f, 1.0f, 0.0f }, { 1.0f, 0.5f, 0.0f },
    { 1.0f, 0.5f, -1.5f }, { 0.5f, 0.8f, -1.5f }, { 1.0f, 0.5f, 0.0f },

    { -0.5f, 0.8f, -1.5f }, { -1.0f, 0.5f, -1.5f }, { -0.5f, 1.0f, 0.0f },
    { -1.0f, 0.5f, -1.5f }, { -1.0f, 0.5f, 0.0f }, { -0.5f, 1.0f, 0.0f },


    // Back
    { -0.5f, 0.8f, -1.5f }, { 0.5f, 0.8f, -1.5f }, { -0.7f, 0.5f, -2.5f },
    { 0.7f, 0.5f, -2.5f }, { -0.7f, 0.5f, -2.5f }, { 0.5f, 0.8f, -1.5f },

    { 0.7f, 0.5f, -2.5f }, { 1.0f, 0.0f, -2.0f }, { -0.7f, 0.5f, -2.5f },
    { -0.7f, 0.5f, -2.5f }, { 1.0f, 0.0f, -2.0f }, { -1.0f, 0.0f, -2.0f },

    { -1.5f, 0.5f, -1.5f }, { -1.0f, 0.5f, -1.5f }, { -1.0f, 0.0f, -1.0f },
    { -1.5f, 0.5f, -1.5f }, { -1.0f, 0.0f, -1.0f }, { -1.5f, 0.0f, -1.0f },

    { 1.0f, 0.5f, -1.5f }, { 1.5f, 0.5f, -1.5f }, { 1.5f, 0.0f, -1.0f },
    { 1.0f, 0.5f, -1.5f }, { 1.5f, 0.0f, -1.0f }, { 1.0f, 0.0f, -1.0f },

    { 0.9f, 0.5f, -1.5f }, { 1.0f, 0.5f, -1.5f }, { 1.0f, 0.0f, -1.0f },
    { 0.9f, 0.5f, -1.5f }, { 1.0f, 0.0f, -1.0f }, { 0.9f, 0.0f, -1.0f },

    { 0.9f, 0.0f, -2.0f }, { 1.0f, 0.0f, -2.0f }, { 1.0f, 0.5f, -1.5f },
    { 0.9f, 0.0f, -2.0f }, { 1.0f, 0.5f, -1.5f }, { 0.9f, 0.5f, -1.5f },

    { -1.0f, 0.5f, -1.5f }, { -0.5f, 0.8f, -1.5f }, { -0.7f, 0.5f, -2.5f },
    { -1.0f, 0.5f, -1.5f }, { -0.7f, 0.5f, -2.5f }, { -1.0f, 0.0f, -2.0f },

    { -1.0f, 0.0f, -2.0f }, { -0.9f, 0.0f, -2.0f }, { -0.9f, 0.5f, -1.5f },
    { -1.0f, 0.0f, -2.0f }, { -0.9f, 0.5f, -1.5f }, { -1.0f, 0.5f, -1.5f },

    { -0.9f, 0.5f, -1.5f }, { -0.9f, 0.0f, -1.0f }, { -1.0f, 0.0f, -1.0f },
    { -0.9f, 0.5f, -1.5f }, { -1.0f, 0.0f, -1.0f }, { -1.0f, 0.5f, -1.5f },

    { 1.0f, 0.5f, -1.5f }, { 1.0f, 0.0f, -2.0f }, { 0.7f, 0.5f, -2.5f },
    { 1.0f, 0.5f, -1.5f }, { 0.7f, 0.5f, -2.5f }, { 0.5f, 0.8f, -1.5f },



    // Sides
    { -1.0f, 0.5f, -1.5f }, { -1.5f, 0.5f, -1.5f }, { -1.5f, 0.5f, 0.0f },
    { -1.0f, 0.5f, -1.5f }, { -1.5f, 0.5f, 0.0f }, { -1.0f, 0.5f, 0.0f },

    { 1.0f, 0.5f, -1.5f }, { 1.0f, 0.5f, 0.0f }, { 1.5f, 0.5f, 0.0f },
    { 1.0f, 0.5f, -1.5f }, { 1.5f, 0.5f, 0.0f }, { 1.5f, 0.5f, -1.5f },

    { -1.5f, 0.5f, -1.5f }, { -1.5f, 0.0f, 0.0f }, { -1.5f, 0.5f, 0.0f },
    { -1.5f, 0.5f, -1.5f }, { -1.5f, 0.0f, -1.0f }, { -1.5f, 0.0f, 0.0f },

    { 1.5f, 0.5f, -1.5f }, { 1.5f, 0.5f, 0.0f }, { 1.5f, 0.0f, 0.0f },
    { 1.5f, 0.5f, -1.5f }, { 1.5f, 0.0f, 0.0f }, { 1.5f, 0.0f, -1.0f },

    { 1.0f, 0.5f, 0.0f }, { 1.0f, 0.5f, 1.5f }, { 1.5f, 0.5f, 1.5f },
    { 1.0f, 0.5f, 0.0f }, { 1.5f, 0.5f, 1.5f }, { 1.5f, 0.5f, 0.0f },

    { -1.0f, 0.5f, 0.0f }, { -1.5f, 0.5f, 0.0f }, { -1.5f, 0.5f, 1.5f },
    { -1.0f, 0.5f, 0.0f }, { -1.5f, 0.5f, 1.5f }, { -1.0f, 0.5f, 1.5f },

    { 1.5f, 0.5f, 0.0f }, { 1.5f, 0.5f, 1.5f }, { 1.5f, 0.0f, 1.0f },
    { 1.5f, 0.5f, 0.0f }, { 1.5f, 0.0f, 1.0f }, { 1.5f, 0.0f, 0.0f },

    { -1.5f, 0.5f, 0.0f }, { -1.5f, 0.0f, 0.0f }, { -1.5f, 0.0f, 1.0f },
    { -1.5f, 0.5f, 0.0f }, { -1.5f, 0.0f, 1.0f }, { -1.5f, 0.5f, 1.5f },


    // Front
    { 1.5f, 0.5f, 1.5f }, { 1.0f, 0.5f, 1.5f }, { 1.0f, 0.0f, 1.0f },
    { 1.5f, 0.5f, 1.5f }, { 1.0f, 0.0f, 1.0f }, { 1.5f, 0.0f, 1.0f },

    { -1.0f, 0.5f, 1.5f }, { -1.5f, 0.5f, 1.5f }, { -1.5f, 0.0f, 1.0f },
    { -1.0f, 0.5f, 1.5f }, { -1.5f, 0.0f, 1.0f }, { -1.0f, 0.0f, 1.0f },

    { -1.0f, 0.5f, 0.0f }, { -1.0f, 0.5f, 1.5f }, { -1.0f, 0.0f, 1.0f },
    { -1.0f, 0.5f, 0.0f }, { -1.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f },

    { 1.0f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f },
    { 1.0f, 0.5f, 0.0f }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.5f, 1.5f },

    { -1.0f, 0.5f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -0.9f, 0.0f, 0.0f },
    { -1.0f, 0.5f, 0.0f }, { -0.9f, 0.0f, 0.0f }, { -0.9f, 0.5f, 0.0f },

    { -0.5f, 0.9f, 0.0f }, { -0.5f, 1.0f, 0.0f }, { -1.0f, 0.5f, 0.0f },
    { -0.5f, 0.9f, 0.0f }, { -1.0f, 0.5f, 0.0f }, { -0.9f, 0.5f, 0.0f },

    { 0.9f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.5f, 0.0f }, 
    { 0.9f, 0.5f, 0.0f }, { 0.9f, 0.0f, 0.0f }, { 1.0f, 0.5f, 0.0f }, 

    { 1.0f, 0.5f, 0.0f }, { 0.5f, 1.0f, 0.0f }, { 0.5f, 0.9f, 0.0f },
    { 0.9f, 0.5f, 0.0f }, { 1.0f, 0.5f, 0.0f }, { 0.5f, 0.9f, 0.0f }, 

    { 0.5f, 1.0f, 0.0f }, { -0.5f, 1.0f, 0.0f }, { -0.5f, 0.9f, 0.0f },
    { 0.5f, 1.0f, 0.0f }, { -0.5f, 0.9f, 0.0f }, { 0.5f, 0.9f, 0.0f },

    // Bottom
    
    { 1.0f, 0.0f, -1.0f }, { 1.5f, 0.0f, -1.0f }, { 1.5f, 0.0f, 0.0f },
    { 1.0f, 0.0f, -1.0f }, { 1.5f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },

    { 1.0f, 0.0f, 0.0f }, { 1.5f, 0.0f, 0.0f }, { 1.5f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 0.0f }, { 1.5f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f },

    { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 1.0f }, { -1.5f, 0.0f, 1.0f },
    { -1.0f, 0.0f, 0.0f }, { -1.5f, 0.0f, 1.0f }, { -1.5f, 0.0f, 0.0f },

    { -1.0f, 0.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { -1.5f, 0.0f, 0.0f },
    { -1.0f, 0.0f, -1.0f }, { -1.5f, 0.0f, 0.0f }, { -1.5f, 0.0f, -1.0f },

    { 0.9f, 0.0f, -1.0f }, { 1.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f },
    { 0.9f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.9f, 0.0f, 0.0f },

    { -0.9f, 0.0f, -1.0f }, { -0.9f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
    { -0.9f, 0.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, -1.0f }
};

std::vector<Vector3> armVertices = {
    // Back cap
    { 0.0f, 0.25f, 0.0f }, { 0.25f, 0.0f, 0.0f }, { -0.25f, 0.0f, 0.0f },
    { 0.25f, 0.0f, 0.0f }, { 0.0f, -0.25f, 0.0f }, { -0.25f, 0.0f, 0.0f },

    // Arm

    { 0.0f, 0.25f, 0.0f }, { 0.0f, 0.25f, 0.5f }, { 0.25f, 0.0f, 0.0f },
    { 0.0f, 0.25f, 0.5f }, { 0.25f, 0.0f, 0.5f }, { 0.25f, 0.0f, 0.0f },

    { 0.0f, 0.25f, 0.0f }, { -0.25f, 0.0f, 0.0f }, { 0.0f, 0.25f, 0.5f },
    { 0.0f, 0.25f, 0.5f }, { -0.25f, 0.0f, 0.0f }, { -0.25f, 0.0f, 0.5f },

    { 0.25f, 0.0f, 0.0f }, { 0.25f, 0.0f, 0.5f }, { 0.0f, -0.25f, 0.0f },
    { 0.0f, -0.25f, 0.0f }, { 0.25f, 0.0f, 0.5f }, {0.0f, -0.25f, 0.5f },

    { -0.25f, 0.0f, 0.0f }, { 0.0f, -0.25f, 0.5f }, { -0.25f, 0.0f, 0.5f },
    { -0.25f, 0.0f, 0.0f }, { 0.0f, -0.25f, 0.0f }, { 0.0f, -0.25f, 0.5f },

    // Wrist

    { 0.7f, 0.15f, 2.0f }, { 0.0f, 0.25f, 0.5f }, { -0.25f, 0.0f, 0.5f },
    { 0.7f, 0.15f, 2.0f }, { -0.25f, 0.0f, 0.5f }, { 0.5f, 0.0f, 2.0f },

    { 0.0f, 0.25f, 0.5f }, { 0.7f, 0.15f, 2.0f },  { 0.25f, 0.0f, 0.5f },
    { 0.25f, 0.0f, 0.5f }, { 0.7f, 0.15f, 2.0f },  { 0.75f, 0.0f, 2.0f },

    { 0.75f, 0.0f, 2.0f }, { 0.0f, -0.25f, 0.5f }, { 0.25f, 0.0f, 0.5f },
    { 0.75f, 0.0f, 2.0f }, { 0.7f, -0.15f, 2.0f }, { 0.0f, -0.25f, 0.5f },

    { 0.7f, -0.15f, 2.0f }, { 0.5f, 0.0f, 2.0f }, { -0.25f, 0.0f, 0.5f },
    { 0.7f, -0.15f, 2.0f }, { -0.25f, 0.0f, 0.5f }, {0.0f, -0.25f, 0.5f },

    // Front rect

    { 0.7f, 0.15f, 3.0f }, { 0.75f, 0.0f, 3.0f }, { 0.7f, 0.15f, 2.0f },
    { 0.7f, 0.15f, 2.0f }, { 0.75f, 0.0f, 3.0f }, { 0.75f, 0.0f, 2.0f },
 
    { 0.7f, 0.15f, 3.0f }, { 0.7f, 0.15f, 2.0f }, { 0.5f, 0.0f, 2.0f },
    { 0.7f, 0.15f, 3.0f }, { 0.5f, 0.0f, 2.0f }, { 0.5f, 0.0f, 3.0f },

    { 0.7f, -0.15f, 2.0f }, { 0.7f, -0.15f, 3.0f }, { 0.5f, 0.0f, 2.0f },
    { 0.5f, 0.0f, 2.0f }, { 0.7f, -0.15f, 3.0f }, { 0.5f, 0.0f, 3.0f },

    { 0.7f, -0.15f, 3.0f }, { 0.7f, -0.15f, 2.0f }, { 0.75f, 0.0f, 3.0f },
    { 0.75f, 0.0f, 2.0f }, { 0.75f, 0.0f, 3.0f }, { 0.7f, -0.15f, 2.0f },

    // Front cap
    
    { 0.5f, 0.0f, 3.0f }, { 0.75f, 0.0f, 3.0f }, { 0.7f, 0.15f, 3.0f }, 
    { 0.5f, 0.0f, 3.0f }, { 0.7f, -0.15f, 3.0f }, { 0.75f, 0.0f, 3.0f }
};

std::vector<Vector3> propellerVertices = {
    // Base
    { 3.5f, 0.35f, 0.0f }, { 3.3f, -0.2f, -0.1f }, { 0.5f, 0.3f, 0.0f },
    { 3.3f, -0.2f, -0.1f }, { 0.5f, -0.1f, 0.0f }, { 0.5f, 0.3f, 0.0f },

    { 3.3f, -0.2f, -0.1f }, { 1.0f, -0.6f, -0.1f }, { 0.5f, -0.1f, 0.0f },

    { 3.4f, 0.3f, 0.1f }, { 3.3f, -0.2f, -0.1f }, { 3.5f, 0.35f, 0.0f },

    { 0.5f, 0.2f, 0.1f }, { 0.5f, 0.3f, 0.0f }, { 0.5f, -0.1f, 0.0f },

    // Top
    { 0.7f, 0.0f, 0.1f }, { 3.3f, -0.2f, -0.1f }, { 3.4f, 0.3f, 0.1f },
    { 0.7f, 0.0f, 0.1f }, { 3.4f, 0.3f, 0.1f }, { 0.5f, 0.2f, 0.1f },
    { 0.7f, 0.0f, 0.1f }, { 0.5f, -0.1f, 0.0f }, { 1.0f, -0.6f, -0.1f },
    { 0.7f, 0.0f, 0.1f }, { 1.0f, -0.6f, -0.1f }, { 3.3f, -0.2f, -0.1f },
    { 0.7f, 0.0f, 0.1f }, { 3.3f, -0.2f, -0.1f }, { 3.4f, 0.3f, 0.1f },
    { 0.7f, 0.0f, 0.1f }, { 0.5f, 0.2f, 0.1f }, { 0.5f, -0.1f, 0.0f },

    // Back
    { 3.4f, 0.3f, 0.1f }, { 3.5f, 0.35f, 0.0f }, { 0.5f, 0.3f, 0.0f },
    { 0.5f, 0.2f, 0.1f }, { 3.4f, 0.3f, 0.1f }, { 0.5f, 0.3f, 0.0f}
};

std::vector<unsigned short> housingIndices = GenerateHardSurfaceIndices(housingVertices.size());
std::vector<Vector3> housingNormals = CalculateNormals(housingVertices, housingIndices);

std::vector<unsigned short> armIndices = GenerateHardSurfaceIndices(armVertices.size());
std::vector<Vector3> armNormals = CalculateNormals(armVertices, armIndices);

std::vector<unsigned short> propellerIndices = GenerateHardSurfaceIndices(propellerVertices.size());
std::vector<Vector3> propellerNormals = CalculateNormals(propellerVertices, propellerIndices);

// Drone Parameters

// Colours
static Color housingColor = Color{ 255, 255, 255, 255 };
static Color chassisColor = Color{ 128, 128, 128, 255 };
static Color armColor = Color{ 25, 31, 52, 255 };
static Color propellerColor = Color{ 0, 0, 0, 255 };

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
Vector3 propellerRotations[] = {
    Vector3{ 90.0f, 0.0f, 0.0f },
    Vector3{ -90.0f, 180.0f, 0.0f },
    Vector3{ 90.0f, 0.0f, 0.0f },
    Vector3{ -90.0f, 180.0f, 0.0f },
    Vector3{ 90.0f, 0.0f, 0.0f },
    Vector3{ -90.0f, 180.0f, 0.0f },
    Vector3{ 90.0f, 0.0f, 0.0f },
    Vector3{ -90.0f, 180.0f, 0.0f }
};

// Positions
Vector3 dronePosition   = Vector3{ 0.0f, 3.0f, 0.0f };
Vector3 housingPosition = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 chassisPosition = Vector3{ 0.0f, 0.0f, 2.0f };
Vector3 armPositions[]  = {
    Vector3{ 1.0f, 0.0f, 1.5f },
    Vector3{ 1.0f, 0.0f, -1.5f },
    Vector3{ -1.0f, 0.0f, 1.5f },
    Vector3{ -1.0f, 0.0f, -1.5f }
};
Vector3 propellerPositions[] = {
    Vector3{ 4.0f, 0.9f, 1.5f },
    Vector3{ 4.0f, 0.9f, 1.5f },
    Vector3{ 4.0f, 0.9f, -1.5f },
    Vector3{ 4.0f, 0.9f, -1.5f },
    Vector3{ -4.0f, 0.9f, 1.5f },
    Vector3{ -4.0f, 0.9f, 1.5f },
    Vector3{ -4.0f, 0.9f, -1.5f },
    Vector3{ -4.0f, 0.9f, -1.5f }
};

// Scales
Vector3 housingScale    = Vector3{ 1.0f, 1.0f, 1.0f };
Vector3 chassisScale    = Vector3{ 1.0f, 1.0f, 1.0f };
Vector3 armScales[]  = {
    Vector3{ 1.0f, 1.0f, 1.0f },
    Vector3{ 1.0f, 1.0f, 1.0f },
    Vector3{ 1.0f, 1.0f, 1.0f },
    Vector3{ 1.0f, 1.0f, 1.0f }
};
Vector3 propellerScales[]  = {
    Vector3{ 0.6f, 0.6f, 0.6f },
    Vector3{ 0.6f, 0.6f, 0.6f },
    Vector3{ 0.6f, 0.6f, 0.6f },
    Vector3{ 0.6f, 0.6f, 0.6f },
    Vector3{ 0.6f, 0.6f, 0.6f },
    Vector3{ 0.6f, 0.6f, 0.6f },
    Vector3{ 0.6f, 0.6f, 0.6f },
    Vector3{ 0.6f, 0.6f, 0.6f }
};


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
    DronePart propellers[8];
} Drone;

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
        arm.color = armColor;
    }
    for (int i = 0; i < std::size(drone.arms); i++) {
        drone.arms[i].position = armPositions[i];
        drone.arms[i].rotation = armRotations[i];
        drone.arms[i].scale = armScales[i];
    }

    for (DronePart propeller : drone.propellers) {
        propeller.color = propellerColor;
    }
    for (int i = 0; i < std::size(drone.propellers); i++) {
        drone.propellers[i].position = propellerPositions[i];
        drone.propellers[i].rotation = propellerRotations[i];
        drone.propellers[i].scale = propellerScales[i];
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
    Mesh dronePropellerMesh = CreateCustomMesh(propellerVertices, propellerNormals, propellerIndices);

    // Models
    Model droneHousingModel = LoadModelFromMesh(droneHousingMesh);
    Model droneChassisModel = LoadModelFromMesh(droneChassisMesh);
    Model droneArmModel     = LoadModelFromMesh(droneArmMesh);
    Model dronePropellerModel = LoadModelFromMesh(dronePropellerMesh);

    // Shader
    Shader lighting = LoadShaderFromMemory(lightingVs, lightingFs);

    droneHousingModel.materials[0].shader   = lighting;
    droneChassisModel.materials[0].shader   = lighting;
    droneArmModel.materials[0].shader       = lighting;
    dronePropellerModel.materials[0].shader      = lighting;

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
            drone.position.x -= 45.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_D)) {
            drone.position.x += 45.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_S)) {
            drone.position.z -= 45.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_W)) {
            drone.position.z += 45.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_C) || IsKeyDown(KEY_LEFT_CONTROL)) {
            drone.position.y -= 45.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_SPACE)) {
            drone.position.y += 45.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_RIGHT)) {
            drone.rotation.y += 45.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_LEFT)) {
            drone.rotation.y -= 45.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_UP)) {
            drone.rotation.x += 45.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_DOWN)) {
            drone.rotation.x -= 45.0f * GetFrameTime();
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

        Matrix propellerWorlds[std::size(drone.propellers)];
        for (int i = 0; i < std::size(drone.propellers); i++) {
            propellerWorlds[i] = 
                ParentChild(
                housingWorld,
                TRS(
                    drone.propellers[i].position,
                    drone.propellers[i].rotation,
                    drone.propellers[i].scale
                )
            );
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                
                // Draw all 3D geometry here
                DrawModelWithWorldMatrix(droneHousingModel, housingWorld, lighting, housingColor);
                DrawModelWithWorldMatrix(droneChassisModel, chassisWorld, lighting, chassisColor);
                for (Matrix armWorld : armWorlds) {
                    DrawModelWithWorldMatrix(droneArmModel, armWorld, lighting, armColor);
                }
                for (Matrix propellerWorld : propellerWorlds) {
                    DrawModelWithWorldMatrix(dronePropellerModel, propellerWorld, lighting, propellerColor);
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
