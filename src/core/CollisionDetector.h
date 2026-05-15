#pragma once

#include <vector>
#include <cmath>
#include <cfloat>
#include <algorithm>

#include "RigidBody.h"
#include "Shape.h"

struct Manifold {
    RigidBody* A = nullptr, *B = nullptr;
    Vec2 normal{};
    float penetration = 0.0f;
    std::vector<Vec2> contacts;
};

struct CollisionDetector {

    static bool test(RigidBody* a, RigidBody* b, Manifold& m) {
        m = {};
        m.A = a; 
        m.B = b;

        ShapeType ta = a->shape->type();
        ShapeType tb = b->shape->type();

        if (ta == ShapeType::CIRCLE  && tb == ShapeType::CIRCLE)
            return circleCircle(a, b, m);

        if (ta == ShapeType::POLYGON && tb == ShapeType::POLYGON)
            return polygonPolygon(a, b, m);

        return false;
    }

    static bool circleCircle(RigidBody* a, RigidBody* b, Manifold& m);

    // =========================================================
    // SAT Query
    // =========================================================

    static float satQueryFaceDirections(
        const Polygon* poly,
        const Polygon* other,
        Vec2 posA,
        float rotA,
        Vec2 posB,
        float rotB,
        int& bestFace)
    {
        auto vA = poly->worldVertices(posA, rotA);
        auto vB = other->worldVertices(posB, rotB);

        float maxSep = -FLT_MAX;

        for (int i = 0; i < (int)vA.size(); ++i) {

            Vec2 edge = vA[(i + 1) % vA.size()] - vA[i];
            Vec2 normal = edge.perp().normalized();

            float minDot = FLT_MAX;

            for (auto& v : vB)
                minDot = std::min(
                    minDot,
                    normal.dot(v - vA[i])
                );

            if (minDot > maxSep) {
                maxSep = minDot;
                bestFace = i;
            }
        }

        return maxSep;
    }

    // =========================================================
    // Face Normal
    // =========================================================

    static Vec2 computeFaceNormal(
        const Polygon* poly,
        float rot,
        int face)
    {
        auto verts =
            poly->worldVertices({0, 0}, rot);

        Vec2 edge =
            verts[(face + 1) % verts.size()]
            - verts[face];

        return edge.perp().normalized();
    }

    // =========================================================
    // Contact Generation
    // =========================================================

    static std::vector<Vec2> clipContactPoints(
        const Polygon* pA,
        const Polygon* pB,
        RigidBody* a,
        RigidBody* b,
        const Vec2& normal)
    {
        return {
            (a->position + b->position) * 0.5f
        };
    }
    static bool polygonPolygon(RigidBody* a, RigidBody* b, Manifold& m);
};