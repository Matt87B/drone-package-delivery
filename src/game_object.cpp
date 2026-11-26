
#include "game_object.h"
#include "raylib.h"
#include "raymath.h"

GameObject::GameObject(Vector3 pos, bool staticObj) : position(pos), isStatic(staticObj) {}

void GameObject::OnCollision(const Vector3& bounceForce) {
    if (!isStatic) {
        velocity = Vector3Add(velocity, bounceForce);
    }
}

Vector3 GameObject::GetPosition() const { return position; }
void GameObject::SetPosition(Vector3 pos) { position = pos; }

Vector3 GameObject::GetVelocity() const { return velocity; }
void GameObject::SetVelocity(Vector3 vel) { velocity = vel; }

bool GameObject::IsStatic() const { return isStatic; }