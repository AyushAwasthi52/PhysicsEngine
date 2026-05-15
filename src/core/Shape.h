#include "AABB.h"
#include "Vec2.h"
#include <vector>


enum class ShapeType { CIRCLE, POLYGON };

struct Shape {
    virtual ~Shape() = default;
    virtual ShapeType   type()              const = 0;
    virtual float       computeArea()       const = 0;
    virtual float       computeInertia(float mass) const = 0;
    virtual AABB        computeAABB(Vec2 pos, float rot) const = 0;
};

struct Circle : Shape {
    float radius = 1.0f;
    ShapeType type() const override { return ShapeType::CIRCLE; }
    float computeArea() const override { return M_PI * radius * radius; }
    float computeInertia(float mass) const override
        { return 0.5f * mass * radius * radius; }
    AABB computeAABB(Vec2 p, float) const override
        { return { p - Vec2{radius,radius}, p + Vec2{radius,radius} }; }
};

struct Polygon : Shape {
    std::vector<Vec2> localVertices; // convex, CCW winding
    ShapeType type() const override { return ShapeType::POLYGON; }
    float computeArea() const override;
    float computeInertia(float mass) const override;
    AABB computeAABB(Vec2 pos, float rot) const override;
    std::vector<Vec2> worldVertices(Vec2 pos, float rot) const;

    // Ensure vertices are CCW and convex (call after editing)
    static Polygon makeBox(float halfW, float halfH);
    static Polygon convexHull(std::vector<Vec2> points);
};