#pragma once
class Vector3;
class Matrix4x4;

Matrix4x4 operator*(const Matrix4x4 &a, const Matrix4x4 &b);
Vector3 operator*(const Matrix4x4 &a, const Vector3 &b);

class Matrix4x4 {
public:
    Matrix4x4(double data[16] = new double[16]{0});

    const double &operator[](int index) const;

    static Matrix4x4 perspective(double fov, double near, double far);
    static Matrix4x4 orthographic(double left, double right, double bottom,
                                  double top, double near, double far);

private:
    double data[16];
};
