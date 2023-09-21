#pragma once

class Vector3;
class Matrix4x4;

Vector3 operator*(const Matrix4x4 &a, const Vector3 &b);

class Vector3 {
public:
    Vector3(double data[4] = new double[4]{0});
    const double &operator[](int index) const;

private:
    double data[3];
};
