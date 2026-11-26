#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raylib.h"
#include "raymath.h"

class GameObject {
protected:
    Vector3 position;
    Vector3 velocity;
    bool isStatic;

public:
    GameObject(Vector3 pos, bool staticObj = false);

    virtual ~GameObject() = default;

    virtual BoundingBox GetBoundingBox() const = 0;

    virtual void OnCollision(const Vector3& bounceForce);

    Vector3 GetPosition() const;
    void SetPosition(Vector3 pos);

    Vector3 GetVelocity() const;
    void SetVelocity(Vector3 vel);
    
    bool IsStatic() const;
};

#endif