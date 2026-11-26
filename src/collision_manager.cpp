#include "raylib.h"
#include "raymath.h"

#include "vector"

#include "collision_manager.h"

CollisionManager::CollisionManager(float r, Vector3 b)  : radius(r), boxSize(b) {}

BoundingBox CollisionManager::GetAABB(Vector3 position) const {
    Vector3 halfSize = Vector3Scale(boxSize, 0.5f);
    return BoundingBox{
        Vector3Subtract(position, halfSize),
        Vector3Add(position, halfSize)
    };
}

bool CollisionManager::HandleGroundCollision(Vector3& position, Vector3& velocity, float groundY, float bounceDampening) {
    float bottomY = position.y - (boxSize.y * 0.5f);

    if (bottomY < groundY) {
        position.y = groundY + (boxSize.y * 0.5f);
        if (velocity.y < 0.0f) {
            velocity.y = -velocity.y * bounceDampening;
            if (fabs(velocity.y) < 0.1f) velocity.y = 0.0f;
        }
        return true;
    }
    return false;
}

bool CollisionManager::CheckObstacleCollision(Vector3 position, BoundingBox obstacle) const {
    return CheckCollisionBoxSphere(obstacle, position, radius);
}

bool CollisionManager::HandleWorldObstacles(Vector3& position, Vector3& velocity, const std::vector<BoundingBox>& obstacles) const {
    bool collided = false;
    
    for (const auto& box : obstacles) {
        if (CheckObstacleCollision(position, box)) {
            collided = true;
            if (Vector3Length(velocity) > 0.01f) {
                Vector3 pushDir = Vector3Negate(Vector3Normalize(velocity));
                position = Vector3Add(position, Vector3Scale(pushDir, 0.1f));
                velocity = Vector3Scale(velocity, -0.2f);
            }
            break;
        }
    }
    return collided;
}

float CollisionManager::GetDistanceToGround(Vector3 position, const std::vector<BoundingBox>& obstacles, float maxDistance) const {
    Ray ray = { position, Vector3{ 0.0f, -1.0f, 0.0f } };
    float closestHit = position.y;

    for (const auto& box : obstacles) {
        RayCollision hit = GetRayCollisionBox(ray, box);
        if (hit.hit && hit.distance < closestHit) {
            closestHit = hit.distance;
        }
    }
    return closestHit;
}
