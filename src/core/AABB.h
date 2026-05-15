#include "Vec2.h"

struct AABB {
    Vec2 min, max;
    bool overlaps(const AABB& o) const;
};
