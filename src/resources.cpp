#include "resources.h"
#include "mesh_utils.h"
#include "raylib.h"

const char* lightingVs = R"(
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

const char* lightingFs = R"(
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


// Colours
Color housingColor = Color{ 255, 255, 255, 255 };
Color chassisColor = Color{ 128, 128, 128, 255 };
Color armColor = Color{ 25, 31, 52, 255 };
Color motorHousingColor = Color{ 0, 0, 0, 255 };
Color rotorShaftColor = Color{ 25, 31, 52, 255 };
Color propColor = Color{ 0, 0, 0, 255 };
Color landingGearColor = Color{ 0, 0, 0, 255 };

// Model vertices
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
std::vector<Vector3> chassisVertices = {
    // Walls
    { -1.0f, 1.0f, 1.0f }, { -1.0f, 1.0f, -1.0f }, { -1.0f, 0.0f, -1.0f },
    { -1.0f, 1.0f, 1.0f }, { -1.0f, 0.0f, -1.0f }, { -1.0f, 0.0f, 1.0f },

    { -1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f, -1.0f },
    { -1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f, -1.0f }, { -1.0f, 0.0f, -1.0f },

    { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, -1.0f },
    { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, -1.0f },

    { 0.6f, 1.5f, -1.0f }, { 1.0f, 1.0f, -1.0f }, { -1.0f, 1.0f, -1.0f },
    { 0.6f, 1.5f, -1.0f }, { -1.0f, 1.0f, -1.0f }, { -0.6f, 1.5f, -1.0f },

    // Top
    { -0.6f, 1.5f, -1.0f }, { -1.0f, 1.0f, -1.0f }, { -1.0f, 1.0f, 1.0f },
    { -0.6f, 1.5f, -1.0f }, { -1.0f, 1.0f, 1.0f }, { -0.6f, 1.5f, 1.0f },

    { 0.6f, 1.5f, -1.0f }, { 0.6f, 1.5f, 1.0f }, { 1.0f, 1.0f, 1.0f },
    { 0.6f, 1.5f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, -1.0f },

    { -0.6f, 1.5f, -1.0f }, { -0.6f, 1.5f, 1.0f }, { 0.6f, 1.5f, 1.0f },
    { -0.6f, 1.5f, -1.0f }, { 0.6f, 1.5f, 1.0f }, { 0.6f, 1.5f, -1.0f },

    // Front

    { -0.6f, 1.5f, 1.0f }, { -1.0f, 1.0f, 1.0f }, { -0.6f, 1.1f, 1.0f },

    { 0.6f, 1.5f, 1.0f }, { 0.6f, 1.1f, 1.0f }, { 1.0f, 1.0f, 1.0f },

    { 0.6f, 1.5f, 1.0f }, { -0.6f, 1.5f, 1.0f }, { -0.6f, 1.1f, 1.0f },
    { 0.6f, 1.5f, 1.0f }, { -0.6f, 1.1f, 1.0f }, { 0.6f, 1.1f, 1.0f },

    { -0.6f, 1.1f, 1.0f }, { -1.0f, 1.0f, 1.0f }, { -1.0f, 0.0f, 1.0f },
    { -0.6f, 1.1f, 1.0f }, { -1.0f, 0.0f, 1.0f }, { -0.6f, 0.0f, 1.0f },

    { 0.6f, 1.1f, 1.0f }, { 0.6f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f },
    { 0.6f, 1.1f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f },

    { 0.6f, 1.1f, 1.0f }, { -0.6f, 1.1f, 1.0f }, { -0.6f, 1.1f, 1.5f },
    { 0.6f, 1.1f, 1.0f }, { -0.6f, 1.1f, 1.5f }, { 0.6f, 1.1f, 1.5f },
    
    { 0.6f, 0.0f, 1.5f }, { -0.6f, 0.0f, 1.5f }, { -0.6f, 0.0f, 1.0f },
    { 0.6f, 0.0f, 1.5f }, { -0.6f, 0.0f, 1.0f }, { 0.6f, 0.0f, 1.0f },

    { -0.6f, 1.1f, 1.5f }, { -0.6f, 1.1f, 1.0f }, { -0.6f, 0.0f, 1.0f },
    { -0.6f, 1.1f, 1.5f }, { -0.6f, 0.0f, 1.0f }, { -0.6f, 0.0f, 1.5f },

    { 0.6f, 0.0f, 1.0f }, { 0.6f, 1.1f, 1.0f }, { 0.6f, 1.1f, 1.5f },
    { 0.6f, 0.0f, 1.0f }, { 0.6f, 1.1f, 1.5f }, { 0.6f, 0.0f, 1.5f },

    { 0.6f, 1.1f, 1.5f }, { 0.4f, 1.1f, 1.7f }, { 0.4f, 0.0f, 1.7f },
    { 0.6f, 1.1f, 1.5f }, { 0.4f, 0.0f, 1.7f }, { 0.6f, 0.0f, 1.5f },

    { -0.4f, 1.1f, 1.7f }, { -0.6f, 1.1f, 1.5f }, { -0.6f, 0.0f, 1.5f },
    { -0.4f, 1.1f, 1.7f }, { -0.6f, 0.0f, 1.5f }, { -0.4f, 0.0f, 1.7f },

    { 0.4f, 1.1f, 1.7f }, { -0.4f, 1.1f, 1.7f }, { -0.4f, 0.0f, 1.7f },
    { 0.4f, 1.1f, 1.7f }, { -0.4f, 0.0f, 1.7f }, { 0.4f, 0.0f, 1.7f },

    { 0.6f, 1.1f, 1.5f }, { -0.6f, 1.1f, 1.5f }, { -0.4f, 1.1f, 1.7f },
    { 0.6f, 1.1f, 1.5f }, { -0.4f, 1.1f, 1.7f }, { 0.4f, 1.1f, 1.7f },

    // Bottom

    { 1.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, -1.0f },
    { 1.0f, 0.0f, 1.0f }, {-1.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, -1.0f },

    { 0.6f, 0.0f, 1.5f }, { -0.6f, 0.0f, 1.5f }, { -0.6f, 0.0f, 1.0f },
    { 0.6f, 0.0f, 1.5f }, { -0.6f, 0.0f, 1.0f }, { 0.6f, 0.0f, 1.0f },

    { 0.4f, 0.0f, 1.7f }, { -0.4f, 0.0f, 1.7f }, { -0.6f, 0.0f, 1.5f },
    { 0.4f, 0.0f, 1.7f }, { -0.6f, 0.0f, 1.5f }, { 0.6f, 0.0f, 1.5f }
};
std::vector<Vector3> armVertices = {

    // Back cap
    { 0.0f, 0.25f, -0.5f }, { 0.25f, 0.0f, -0.5f }, { -0.25f, 0.0f, -0.5f },
    { 0.25f, 0.0f, -0.5f }, { 0.0f, -0.25f, -0.5f }, { -0.25f, 0.0f, -0.5f },

    // Arm

    { 0.0f, 0.25f, -0.5f }, { 0.0f, 0.25f, 0.5f }, { 0.25f, 0.0f, -0.5f },
    { 0.0f, 0.25f, 0.5f }, { 0.25f, 0.0f, 0.5f }, { 0.25f, 0.0f, -0.5f },

    { 0.0f, 0.25f, -0.5f }, { -0.25f, 0.0f, -0.5f }, { 0.0f, 0.25f, 0.5f },
    { 0.0f, 0.25f, 0.5f }, { -0.25f, 0.0f, -0.5f }, { -0.25f, 0.0f, 0.5f },

    { 0.25f, 0.0f, -0.5f }, { 0.25f, 0.0f, 0.5f }, { 0.0f, -0.25f, -0.5f },
    { 0.0f, -0.25f, -0.5f }, { 0.25f, 0.0f, 0.5f }, {0.0f, -0.25f, 0.5f },

    { -0.25f, 0.0f, -0.5f }, { 0.0f, -0.25f, 0.5f }, { -0.25f, 0.0f, 0.5f },
    { -0.25f, 0.0f, -0.5f }, { 0.0f, -0.25f, -0.5f }, { 0.0f, -0.25f, 0.5f },

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
std::vector<Vector3> propRightVertices = {
    // Base
    { 3.1f, 0.35f, 0.0f }, { 2.9f, -0.2f, -0.1f }, { 0.1f, 0.3f, 0.0f },
    { 2.9f, -0.2f, -0.1f }, { 0.1f, -0.1f, 0.0f }, { 0.1f, 0.3f, 0.0f },

    { 2.9f, -0.2f, -0.1f }, { 0.6f, -0.6f, -0.1f }, { 0.1f, -0.1f, 0.0f },

    { 3.0f, 0.3f, 0.1f }, { 2.9f, -0.2f, -0.1f }, { 3.1f, 0.35f, 0.0f },

    { 0.1f, 0.2f, 0.1f }, { 0.1f, 0.3f, 0.0f }, { 0.1f, -0.1f, 0.0f },

    // Top
    { 0.3f, 0.0f, 0.1f }, { 2.9f, -0.2f, -0.1f }, { 3.0f, 0.3f, 0.1f },
    { 0.3f, 0.0f, 0.1f }, { 3.0f, 0.3f, 0.1f }, { 0.1f, 0.2f, 0.1f },
    { 0.3f, 0.0f, 0.1f }, { 0.1f, -0.1f, 0.0f }, { 0.6f, -0.6f, -0.1f },
    { 0.3f, 0.0f, 0.1f }, { 0.6f, -0.6f, -0.1f }, { 2.9f, -0.2f, -0.1f },
    { 0.3f, 0.0f, 0.1f }, { 2.9f, -0.2f, -0.1f }, { 3.0f, 0.3f, 0.1f },
    { 0.3f, 0.0f, 0.1f }, { 0.1f, 0.2f, 0.1f }, { 0.1f, -0.1f, 0.0f },

    // Back
    { 3.0f, 0.3f, 0.1f }, { 3.1f, 0.35f, 0.0f }, { 0.1f, 0.3f, 0.0f },
    { 0.1f, 0.2f, 0.1f }, { 3.0f, 0.3f, 0.1f }, { 0.1f, 0.3f, 0.0f } 
};
std::vector<Vector3> propLeftVertices = {
    // Base
    { -0.1f, 0.3f, 0.0f }, { -2.9f, -0.2f, -0.1f }, { -3.1f, 0.35f, 0.0f },
    { -0.1f, 0.3f, 0.0f }, { -0.1f, -0.1f, 0.0f }, { -2.9f, -0.2f, -0.1f },

    { -0.1f, -0.1f, 0.0f }, { -0.6f, -0.6f, -0.1f }, { -2.9f, -0.2f, -0.1f },

    { -3.1f, 0.35f, 0.0f }, { -2.9f, -0.2f, -0.1f }, { -3.0f, 0.3f, 0.1f },

    { -0.1f, -0.1f, 0.0f }, { -0.1f, 0.3f, 0.0f }, { -0.1f, 0.2f, 0.1f },

    // Top
    { -3.0f, 0.3f, 0.1f }, { -2.9f, -0.2f, -0.1f }, { -0.3f, 0.0f, 0.1f },
    { -0.1f, 0.2f, 0.1f }, { -3.0f, 0.3f, 0.1f }, { -0.3f, 0.0f, 0.1f },
    { -0.6f, -0.6f, -0.1f }, { -0.1f, -0.1f, 0.0f }, { -0.3f, 0.0f, 0.1f },
    { -2.9f, -0.2f, -0.1f }, { -0.6f, -0.6f, -0.1f }, { -0.3f, 0.0f, 0.1f },
    { -3.0f, 0.3f, 0.1f }, { -2.9f, -0.2f, -0.1f }, { -0.3f, 0.0f, 0.1f },
    { -0.1f, -0.1f, 0.0f }, { -0.1f, 0.2f, 0.1f }, { -0.3f, 0.0f, 0.1f },

    // Back
    { -0.1f, 0.3f, 0.0f }, { -3.1f, 0.35f, 0.0f }, { -3.0f, 0.3f, 0.1f },
    { -0.1f, 0.3f, 0.0f }, { -3.0f, 0.3f, 0.1f }, { -0.1f, 0.2f, 0.1f }
};
std::vector<Vector3> landingGearVertices = {
    { -1.25f, -0.4f, 1.25f }, { -1.5f, 0.0f, 1.25f }, { -1.5f, 0.0f, -1.25f },
    { -1.25f, -0.4f, 1.25f }, { -1.5f, 0.0f, -1.25f }, { -1.25f, -0.4f, -1.25f },

    { -1.0f, -0.5f, 1.25f }, { -1.25f, -0.4f, 1.25f }, { -1.25f, -0.4f, -1.25f },
    { -1.0f, -0.5f, 1.25f }, { -1.25f, -0.4f, -1.25f }, { -1.0f, -0.5f, -1.25f },

    { -1.5f, 0.0f, 1.25f }, { -1.0f, 0.0f, 1.25f }, { -1.0f, 0.0f, -1.25f },
    { -1.5f, 0.0f, 1.25f }, { -1.0f, 0.0f, -1.25f }, { -1.5f, 0.0f, -1.25f },

    { -1.0f, 0.0f, 1.25f }, { -1.0f, -0.5f, 1.25f }, { -1.0f, -0.5f, -1.25f },
    { -1.0f, 0.0f, 1.25f }, { -1.0f, -0.5f, -1.25f }, { -1.0f, 0.0f, -1.25f },

    { 1.5f, 0.0f, 1.25f }, { 1.25f, -0.4f, 1.25f }, { 1.25f, -0.4f, -1.25f },
    { 1.5f, 0.0f, 1.25f }, { 1.25f, -0.4f, -1.25f }, { 1.5f, 0.0f, -1.25f },

    { 1.25f, -0.4f, 1.25f }, { 1.0f, -0.5f, 1.25f }, { 1.0f, -0.5f, -1.25f },
    { 1.25f, -0.4f, 1.25f }, { 1.0f, -0.5f, -1.25f }, { 1.25f, -0.4f, -1.25f },

    { 1.0f, 0.0f, -1.25f }, { 1.0f, -0.5f, -1.25f }, { 1.0f, -0.5f, 1.25f },
    { 1.0f, 0.0f, -1.25f }, { 1.0f, -0.5f, 1.25f }, {1.0f, 0.0f, 1.25f },

    { 1.0f, 0.0f, 1.25f }, { 1.5f, 0.0f, 1.25f }, { 1.5f, 0.0f, -1.25f },
    { 1.0f, 0.0f, 1.25f }, { 1.5f, 0.0f, -1.25f }, { 1.0f, 0.0f, -1.25f },

    { -1.0f, 0.0f, -1.75f }, { 1.0f, 0.0f, -1.75f }, { 1.0f, -0.4f, -1.5f },
    { -1.0f, 0.0f, -1.75f }, { 1.0f, -0.4f, -1.5f }, { -1.0f, -0.4f, -1.5f },

    { -1.0f, -0.4f, -1.5f }, { 1.0f, -0.4f, -1.5f }, { 1.0f, -0.5f, -1.25f },
    { -1.0f, -0.4f, -1.5f }, { 1.0f, -0.5f, -1.25f }, { -1.0f, -0.5f, -1.25f },

    { -1.0f, -0.5f, -1.25f }, { 1.0f, -0.5f, -1.25f }, { 1.0f, 0.0f, -1.25f },
    { -1.0f, -0.5f, -1.25f }, { 1.0f, 0.0f, -1.25f }, { -1.0f, 0.0f, -1.25f },

    { -1.0f, 0.0f, -1.25f }, { 1.0f, 0.0f, -1.25f }, { 1.0f, 0.0f, -1.75f },
    { -1.0f, 0.0f, -1.25f }, { 1.0f, 0.0f, -1.75f }, { -1.0f, 0.0f, -1.75f },

    { -1.0f, 0.0f, 1.75f }, { 1.0f, 0.0f, 1.75f }, { 1.0f, 0.0f, 1.25f },
    { -1.0f, 0.0f, 1.75f }, { 1.0f, 0.0f, 1.25f }, { -1.0f, 0.0f, 1.25f },

    { -1.0f, 0.0f, 1.25f }, { 1.0f, 0.0f, 1.25f }, { 1.0f, -0.5f, 1.25f },
    { -1.0f, 0.0f, 1.25f }, { 1.0f, -0.5f, 1.25f }, { -1.0f, -0.5f, 1.25f },

    { -1.0f, -0.4f, 1.5f }, { 1.0f, -0.4f, 1.5f }, { 1.0f, 0.0f, 1.75f },
    { -1.0f, -0.4f, 1.5f }, { 1.0f, 0.0f, 1.75f }, { -1.0f, 0.0f, 1.75f },

    { -1.0f, -0.5f, 1.25f }, { 1.0f, -0.5f, 1.25f }, { 1.0f, -0.4f, 1.5f },
    { -1.0f, -0.5f, 1.25f }, { 1.0f, -0.4f, 1.5f }, { -1.0f, -0.4f, 1.5f },

    { -1.25f, -0.4f, 1.25f }, { -1.0f, -0.4f, 1.5f }, { -1.0f, 0.0f, 1.75f },
    { -1.25f, -0.4f, 1.25f }, { -1.0f, 0.0f, 1.75f }, { -1.5f, 0.0f, 1.25f },

    { 1.0f, -0.4f, 1.5f }, { 1.25f, -0.4f, 1.25f }, { 1.5f, 0.0f, 1.25f },
    { 1.0f, -0.4f, 1.5f }, { 1.5f, 0.0f, 1.25f }, { 1.0f, 0.0f, 1.75f },

    { -1.5f, 0.0f, -1.25f }, { -1.0f, 0.0f, -1.75f }, { -1.0f, -0.4f, -1.5f },
    { -1.5f, 0.0f, -1.25f }, { -1.0f, -0.4f, -1.5f }, { -1.25f, -0.4f, -1.25f },

    { 1.0f, 0.0f, -1.75f }, { 1.5f, 0.0f, -1.25f }, { 1.25f, -0.4f, -1.25f },
    { 1.0f, 0.0f, -1.75f }, { 1.25f, -0.4f, -1.25f }, { 1.0f, -0.4f, -1.5f },

    { 1.0f, 0.0f, -1.25f }, { 1.5f, 0.0f, -1.25f }, { 1.0f, 0.0f, -1.75f },
    { -1.0f, 0.0f, -1.25f }, { -1.0f, 0.0f, -1.75f }, { -1.5f, 0.0f, -1.25f },

    { -1.5f, 0.0f, 1.25f }, { -1.0f, 0.0f, 1.75f }, { -1.0f, 0.0f, 1.25f },
    { 1.0f, 0.0f, 1.75f }, { 1.5f, 0.0f, 1.25f }, { 1.0f, 0.0f, 1.25f },

    { -1.0f, -0.4f, 1.5f }, { -1.25f, -0.4f, 1.25f }, { -1.0f, -0.5f, 1.25f },

    { 1.25f, -0.4f, 1.25f }, { 1.0f, -0.4f, 1.5f }, { 1.0f, -0.5f, 1.25f },

    { -1.0f, -0.5f, -1.25f }, { -1.25f, -0.4f, -1.25f }, { -1.0f, -0.4f, -1.5f },
    
    { 1.0f, -0.5f, -1.25f }, { 1.0f, -0.4f, -1.5f }, { 1.25f, -0.4f, -1.25f }
};

// Model texcoords and indices
std::vector<unsigned short> housingIndices = GenerateHardSurfaceIndices(housingVertices.size());
std::vector<Vector3> housingNormals = CalculateNormals(housingVertices, housingIndices);

std::vector<unsigned short> armIndices = GenerateHardSurfaceIndices(armVertices.size());
std::vector<Vector3> armNormals = CalculateNormals(armVertices, armIndices);

std::vector<unsigned short> chassisIndices = GenerateHardSurfaceIndices(chassisVertices.size());
std::vector<Vector3> chassisNormals = CalculateNormals(chassisVertices, chassisIndices);

std::vector<unsigned short> propRightIndices = GenerateHardSurfaceIndices(propRightVertices.size());
std::vector<Vector3> propRightNormals = CalculateNormals(propRightVertices, propRightIndices);

std::vector<unsigned short> propLeftIndices = GenerateHardSurfaceIndices(propLeftVertices.size());
std::vector<Vector3> propLeftNormals = CalculateNormals(propLeftVertices, propLeftIndices);

std::vector<unsigned short> landingGearIndices = GenerateHardSurfaceIndices(landingGearVertices.size());
std::vector<Vector3> landingGearNormals = CalculateNormals(landingGearVertices, landingGearIndices);

// Angles
Vector3 droneRotation   = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 housingRotation = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 chassisRotation = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 landingGearRotation = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 armRotations[] = {
    Vector3{ 0.0f, 80.0f, 90.0f },
    Vector3{ 0.0f, 110.0f, 90.0f },
    Vector3{ 0.0f, -80.0f, 90.0f },
    Vector3{ 0.0f, -110.0f, 90.0f }
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
Vector3 chassisPosition = Vector3{ 0.0f, 0.0f, -0.2f };
Vector3 landingGearPosition = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 armPositions[]  = {
    Vector3{ 1.0f, 0.2f, 1.5f },
    Vector3{ 1.0f, 0.2f, -1.5f },
    Vector3{ -1.0f, 0.2f, 1.5f },
    Vector3{ -1.0f, 0.2f, -1.5f }
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
Vector3 chassisScale    = Vector3{ 0.9f, 0.5f, 1.2f };
Vector3 armScale        = Vector3{ 1.0f, 1.0f, 1.0f };
Vector3 landingGearScale = Vector3{ 0.9f, 0.5f, 1.0f };
Vector3 motorHousingScale = Vector3{ 0.5f, 0.5f, 0.5f };
Vector3 rotorShaftScale = Vector3{ 1.0f, 1.0f, 1.0f };
Vector3 propScale       = Vector3{ 1.4f, 1.0f, 1.4f };