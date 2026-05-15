#pragma once

#include <math.h>

struct Vec2 {
    float x = 0, y = 0;
    Vec2(float x = 0, float y = 0) : x(x), y(y) {}

    Vec2  operator+(const Vec2& o) const { return {x+o.x, y+o.y}; }
    Vec2  operator-(const Vec2& o) const { return {x-o.x, y-o.y}; }
    Vec2  operator*(float s)       const { return {x*s,   y*s};   }
    Vec2& operator+=(const Vec2& o)      { x+=o.x; y+=o.y; return *this; }
    Vec2& operator-=(const Vec2& o)      { x-=o.x; y-=o.y; return *this; }
    Vec2& operator*=(const float s)     { x*=s; y*=s; return *this; }
    Vec2 operator-() const { return {-x, -y}; }

    float dot(const Vec2& o) const { return x*o.x + y*o.y; }
    float cross(const Vec2& o) const { return x*o.y - y*o.x; }
    float lengthSq()    const { return dot(*this); }
    float length()      const { return std::sqrt(lengthSq()); }
    Vec2  normalized()  const { float l = length(); return l > 1e-6f ? *this*(1/l) : Vec2{}; }
    Vec2  perp()        const { return {-y, x}; } // rotate 90 deg CCW

    // Rotate by angle (radians)
    Vec2 rotated(float angle) const {
        float c = std::cos(angle), s = std::sin(angle);
        return { x*c - y*s, x*s + y*c };
    }
};