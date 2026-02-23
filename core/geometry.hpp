#pragma once

#include <cmath>

namespace vine {

// 3D vector / point
struct Vec3 {
    float x{0.f}, y{0.f}, z{0.f};

    Vec3() = default;
    Vec3(float x, float y, float z);

    Vec3  operator+(const Vec3& o) const;
    Vec3  operator-(const Vec3& o) const;
    Vec3  operator*(float s)       const;
    Vec3& operator+=(const Vec3& o);

    float dot(const Vec3& o)   const;
    Vec3  cross(const Vec3& o) const;
    float length()             const;
    Vec3  normalized()         const;
};

// Spline control point: position + tube radius for vine segments
struct ControlPoint {
    Vec3  position;
    float radius{0.05f};

    ControlPoint() = default;
    ControlPoint(Vec3 pos, float r = 0.05f);
};

// Evaluate a cubic Bezier curve at t in [0, 1]
Vec3 bezierEvaluate(const Vec3& p0, const Vec3& p1,
                    const Vec3& p2, const Vec3& p3,
                    float t);

} // namespace vine
