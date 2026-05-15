#include "AABB.h"

bool AABB::overlaps(const AABB& o) const {
        return max.x >= o.min.x && min.x <= o.max.x
            && max.y >= o.min.y && min.y <= o.max.y;
    }