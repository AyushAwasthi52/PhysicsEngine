#pragma once

#include <cmath>
#include <algorithm>

#include "RigidBody.h"
#include "CollisionDetector.h"

struct CollisionResolver {

    static void resolve(
        RigidBody* a,
        RigidBody* b,
        const Manifold& m,
        float dt
    );
};