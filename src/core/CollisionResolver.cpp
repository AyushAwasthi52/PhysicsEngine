#include "CollisionResolver.h"

void CollisionResolver::resolve(RigidBody* a, RigidBody* b,
    const Manifold& m, float dt) {

    for (Vec2 contact : m.contacts) {
        Vec2 rA = contact - a->position;
        Vec2 rB = contact - b->position;

        // Relative velocity at contact point
        Vec2 vA_c = a->velocity + Vec2{-a->angularVelocity*rA.y,
                                        a->angularVelocity*rA.x};
        Vec2 vB_c = b->velocity + Vec2{-b->angularVelocity*rB.y,
                                        b->angularVelocity*rB.x};
        Vec2 vRel = vB_c - vA_c;

        float vRelN = vRel.dot(m.normal);
        if (vRelN > 0) continue; // separating — no impulse needed

        float e = std::min(a->restitution, b->restitution);

        // Effective mass along normal
        float rAxN  = rA.cross(m.normal);
        float rBxN  = rB.cross(m.normal);
        float effMass = a->invMass + b->invMass
                      + rAxN*rAxN*a->invInertia
                      + rBxN*rBxN*b->invInertia;

        float jN = -(1.0f + e) * vRelN / (effMass * m.contacts.size());

        // Normal impulse
        Vec2 impulse = m.normal * jN;
        a->applyImpulse(-impulse, rA);
        b->applyImpulse( impulse, rB);

        // --- Friction ---
        vRel = (b->velocity + Vec2{-b->angularVelocity*rB.y,
                                    b->angularVelocity*rB.x})
             - (a->velocity + Vec2{-a->angularVelocity*rA.y,
                                    a->angularVelocity*rA.x});
        Vec2 tangent = (vRel - m.normal * vRel.dot(m.normal)).normalized();

        float vRelT = vRel.dot(tangent);
        float jT    = -vRelT / (effMass * m.contacts.size());

        float mu = std::sqrt(a->staticFriction * b->staticFriction);
        Vec2  frictionImpulse;
        if (std::abs(jT) < jN * mu)
            frictionImpulse = tangent * jT;            // static friction
        else {
            float dynMu = std::sqrt(a->dynamicFriction*b->dynamicFriction);
            frictionImpulse = tangent * (-jN * dynMu); // kinetic friction
        }
        a->applyImpulse(-frictionImpulse, rA);
        b->applyImpulse( frictionImpulse, rB);
    }

    // Positional correction (Baumgarte) — removes sinking
    const float SLOP    = 0.01f;
    const float PERCENT = 0.4f;
    Vec2 correction = m.normal
        * (std::max(m.penetration - SLOP, 0.0f) / (a->invMass+b->invMass))
        * PERCENT;
    a->position -= correction * a->invMass;
    b->position += correction * b->invMass;
}