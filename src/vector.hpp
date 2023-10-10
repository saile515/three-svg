#pragma once

#include <array>

class Vector4;
class Matrix4x4;

Vector4 operator*(const Matrix4x4 &a, const Vector4 &b);

class Vector3 {
public:
    Vector3(std::array<double, 3> = std::array<double, 3>({0, 0, 0}));
    [[nodiscard]] const double &operator[](int index) const;

private:
    double data[3];
};

class Vector4 {
public:
    Vector4(double data[4] = new double[4]{0});
    Vector4(Vector3 vector, double w);
    [[nodiscard]] const double &operator[](int index) const;

private:
    double data[4];
};
