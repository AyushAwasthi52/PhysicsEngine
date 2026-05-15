#include "CollisionDetector.h"

bool CollisionDetector::circleCircle(
    RigidBody* a, RigidBody* b, Manifold& m) {

    auto* ca = static_cast<Circle*>(a->shape.get());
    auto* cb = static_cast<Circle*>(b->shape.get());
    Vec2 delta   = b->position - a->position;
    float distSq = delta.lengthSq();
    float radSum = ca->radius + cb->radius;

    if (distSq >= radSum * radSum) return false;

    float dist    = std::sqrt(distSq);
    m.normal      = dist > 1e-6f ? delta * (1.0f/dist) : Vec2{1,0};
    m.penetration = radSum - dist;
    m.contacts    = { a->position + m.normal * ca->radius };
    return true;
}

bool CollisionDetector::polygonPolygon(
    RigidBody* a, RigidBody* b, Manifold& m) {
    auto* pA = static_cast<Polygon*>(a->shape.get());
    auto* pB = static_cast<Polygon*>(b->shape.get());

    int faceA, faceB;
    float sepA = satQueryFaceDirections(pA, pB, a->position, a->rotation,
                                         b->position, b->rotation, faceA);
    if (sepA > 0.0f) return false;

    float sepB = satQueryFaceDirections(pB, pA, b->position, b->rotation,
                                         a->position, a->rotation, faceB);
    if (sepB > 0.0f) return false;

    // Choose reference face (least penetration = most reliable normal)
    if (sepA >= sepB * 0.95f) {
        m.normal      = computeFaceNormal(pA, a->rotation, faceA);
        m.penetration = -sepA;
    } else {
        m.normal      = -computeFaceNormal(pB, b->rotation, faceB);
        m.penetration = -sepB;
    }
    m.contacts = clipContactPoints(pA, pB, a, b, m.normal);
    return !m.contacts.empty();
}
