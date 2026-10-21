#include "resources.h"
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

