#include "RigidBody.h"

void RigidBody::integrate(float dt) {
    if (invMass == 0.0f) return; // static body

    // Apply damping first (velocity-dependent)
    velocity       *= (1.0f - linearDamping  * dt);
    angularVelocity *= (1.0f - angularDamping * dt);

    // Semi-implicit: update velocity with force, then position with new velocity
    Vec2 acceleration = force * invMass;
    velocity  += acceleration * dt;
    position  += velocity    * dt;  // uses NEW velocity

    float angularAccel = torque * invInertia;
    angularVelocity += angularAccel * dt;
    rotation        += angularVelocity * dt;

    clearForces();
}

void RigidBody::applyImpulse(Vec2 j, Vec2 r) {
    velocity        += j * invMass;
    angularVelocity += r.cross(j) * invInertia;
}