// #include "PhysicsWorld.h"
// #include <vector>

// // PhysicsWorld::broadPhase() — O(n^2) for < 500 bodies,
// // replace with dynamic BVH or spatial hash for 500+
// std::vector<BodyPair> PhysicsWorld::broadPhase() {
//     std::vector<BodyPair> pairs;
//     for (size_t i = 0; i < bodies.size(); ++i)
//         for (size_t j = i+1; j < bodies.size(); ++j)
//             if (bodies[i]->computeAABB().overlaps(
//                 bodies[j]->computeAABB()))
//                 pairs.push_back({bodies[i].get(), bodies[j].get()});
//     return pairs;
// }