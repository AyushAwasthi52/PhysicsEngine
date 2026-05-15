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

void RigidBody::setMass(float m)
{
    mass = m;

    if (mass <= 0.0f)
    {
        invMass = 0.0f;
        return;
    }

    invMass = 1.0f / mass;

    if (shape)
    {
        inertia = shape->computeInertia(mass);

        if (inertia > 0.0f)
            invInertia = 1.0f / inertia;
        else
            invInertia = 0.0f;
    }
}

void RigidBody::applyForce(Vec2 f)
{
    force += f;
}

void RigidBody::clearForces()
{
    force = {0.0f, 0.0f};
    torque = 0.0f;
}

AABB RigidBody::computeAABB() const
{
    if (shape)
        return shape->computeAABB(position, rotation);

    return { position, position };
}