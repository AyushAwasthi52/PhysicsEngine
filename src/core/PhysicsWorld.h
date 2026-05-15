#include "Vec2.h"
#include <vector>
#include "RigidBody.h"
#include <memory>
#include <CollisionDetector.h>
#include <CollisionResolver.h>

class PhysicsWorld {
public:
    Vec2  gravity        = {0.0f, -980.0f}; // pixels/s^2 (screen-space)
    int   solverIterations = 8;              // impulse solver passes

    std::vector<std::unique_ptr<RigidBody>> bodies;

    RigidBody* addBody(std::unique_ptr<RigidBody> b)
        { bodies.push_back(std::move(b)); return bodies.back().get(); }

    void step(float dt) {
        // 1. Apply gravity to dynamic bodies
        for (auto& b : bodies)
            if (b->invMass > 0)
                b->applyForce(gravity * b->mass);

        // 2. Integrate velocities & positions
        for (auto& b : bodies) b->integrate(dt);

        // 3. Broad phase — candidate pairs
        auto pairs = broadPhase();

        // 4. Narrow phase — generate manifolds
        std::vector<Manifold> manifolds;
        for (auto& [a, b] : pairs) {
            Manifold m;
            if (CollisionDetector::test(a, b, m))
                manifolds.push_back(m);
        }

        // 5. Resolve — multiple iterations for stacking stability
        for (int iter = 0; iter < solverIterations; ++iter)
            for (auto& m : manifolds)
                CollisionResolver::resolve(m.A, m.B, m, dt);
    }
private:

    std::vector<std::pair<RigidBody*, RigidBody*>>
    broadPhase()
    {
        std::vector<std::pair<RigidBody*, RigidBody*>> pairs;

        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = i + 1; j < bodies.size(); ++j) {

                RigidBody* a = bodies[i].get();
                RigidBody* b = bodies[j].get();

                if (a->computeAABB().overlaps(
                    b->computeAABB()))
                {
                    pairs.push_back({a, b});
                }
            }
        }

        return pairs;
    }
};