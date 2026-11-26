#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "raylib.h"
#include "vector"

class CollisionManager {
public:
    float radius;
    Vector3 boxSize;

    CollisionManager(float r, Vector3 b);

    BoundingBox GetAABB(Vector3 position) const;

    bool HandleGroundCollision(Vector3& position, Vector3& velocity, float groundY = 0.0f, float bounceDampening = 0.3f);
    bool CheckObstacleCollision(Vector3 position, BoundingBox obstacle) const;
    bool HandleWorldObstacles(Vector3& position, Vector3& velocity, const std::vector<BoundingBox>& obstacles) const;
    float GetDistanceToGround(Vector3 position, const std::vector<BoundingBox>& obstacles, float maxDistance = 100.0f) const;
};

#endif