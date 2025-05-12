#ifndef VECTOR_H
#define VECTOR_H

class Vector {
public:
    Vector(double x = 0, double y = 0) noexcept;
    static Vector add(const Vector& v1, const Vector& v2) noexcept;
    double x, y;
};

#endif // VECTOR_H
