#include <Shape.h>
#include <algorithm>

Polygon Polygon::convexHull(std::vector<Vec2> pts) {
    // 1. Find lowest-y (then leftmost) pivot
    auto pivot = *std::min_element(pts.begin(), pts.end(),
    [](auto& a, auto& b){ return a.y < b.y || (a.y==b.y && a.x<b.x); });

    // 2. Sort by polar angle relative to pivot
    std::sort(pts.begin(), pts.end(), [&](auto& a, auto& b){
        Vec2 da = a - pivot, db = b - pivot;
        float cross = da.cross(db);
        if (std::abs(cross) > 1e-6f) return cross > 0;
        return da.lengthSq() < db.lengthSq();
    });

    // 3. Graham scan
    std::vector<Vec2> hull;
    for (auto& p : pts) {
        while (hull.size() >= 2 &&
               (hull.back()-hull[hull.size()-2]).cross(p-hull[hull.size()-2]) <= 0)
            hull.pop_back();
        hull.push_back(p);
    }

    Polygon poly;
    poly.localVertices = hull;
    return poly;
}