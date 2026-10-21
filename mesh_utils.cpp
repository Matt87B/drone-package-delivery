#include "vector"

#include "mesh_utils.h"

#include "raylib.h"
#include "raymath.h"


// Create a custom mesh
Mesh CreateCustomMesh(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<unsigned short>& indices) {
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
    // Indices
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

// Generate indices for unshared vertices
std::vector<unsigned short> GenerateHardSurfaceIndices(size_t vertexCount) {
    std::vector<unsigned short> indices(vertexCount);
    for (size_t i = 0; i < vertexCount; i++) {
        indices[i] = static_cast<unsigned short>(i);
    }
    return indices;
}

// Calculates outwards facing normals for mesh
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