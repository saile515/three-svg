#pragma once
class Vector3;
class Matrix4x4;

Matrix4x4 operator*(const Matrix4x4 &a, const Matrix4x4 &b);

class Matrix4x4 {
public:
    Matrix4x4(double data[16] = new double[16]{0});

    [[nodiscard]] const double &operator[](int index) const;

    [[nodiscard]] static Matrix4x4 perspective(double fov, double near, double far);
    [[nodiscard]] static Matrix4x4 orthographic(double left, double right, double bottom, double top, double near, double far);
    [[nodiscard]] static Matrix4x4 position(Vector3 vector);
    [[nodiscard]] static Matrix4x4 rotation(Vector3 vector);
    [[nodiscard]] static Matrix4x4 scale(Vector3 vector);

private:
    double data[16];
};
