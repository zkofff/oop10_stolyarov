#include "vector.h"

Vector::Vector(double x, double y) noexcept : x(x), y(y) {}

Vector Vector::add(const Vector& v1, const Vector& v2) noexcept {
    return Vector(v1.x + v2.x, v1.y + v2.y);
}
