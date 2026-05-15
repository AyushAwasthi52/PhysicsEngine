#include "Vec2.h"
#include "AABB.h"
#include <memory>
#include "Shape.h"

class RigidBody {
public:
    // --- Transform ---
    Vec2  position    = {0, 0};
    float rotation    = 0.0f;   // radians

    // --- Linear dynamics ---
    Vec2  velocity    = {0, 0};
    Vec2  force       = {0, 0};
    float mass        = 1.0f;
    float invMass     = 1.0f;   // 0 = static (infinite mass)

    // --- Angular dynamics ---
    float angularVelocity = 0.0f;
    float torque          = 0.0f;
    float inertia         = 1.0f;
    float invInertia      = 1.0f;

    // --- Material ---
    float restitution   = 0.4f;  // bounciness [0,1]
    float staticFriction  = 0.5f;
    float dynamicFriction = 0.3f;
    float linearDamping   = 0.01f;
    float angularDamping  = 0.01f;

    // --- Geometry ---
    std::unique_ptr<Shape> shape;

    // --- Helpers ---
    void setMass(float m);       // updates invMass & inertia
    void applyForce(Vec2 f);
    void applyImpulse(Vec2 j, Vec2 contactPoint);
    void integrate(float dt);    // semi-implicit Euler
    void clearForces();
    AABB computeAABB() const;
};