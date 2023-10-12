#pragma once

#include <array>

class Vector3;
class Matrix4x4;

Matrix4x4 operator*(const Matrix4x4 &a, const Matrix4x4 &b);
Matrix4x4 operator*(double a, const Matrix4x4 &b);

class Matrix4x4 {
public:
    Matrix4x4(std::array<double, 16> = std::array<double, 16>({0}));

    void print() const;

    [[nodiscard]] Matrix4x4 inverse();

    [[nodiscard]] const double &operator[](int index) const;

    [[nodiscard]] static Matrix4x4 perspective(double fov, double width, double height, double near, double far);
    [[nodiscard]] static Matrix4x4 orthographic(double left, double right, double bottom, double top, double near, double far);
    [[nodiscard]] static Matrix4x4 position(Vector3 vector);
    [[nodiscard]] static Matrix4x4 rotation(Vector3 vector);
    [[nodiscard]] static Matrix4x4 scale(Vector3 vector);

    const std::array<double, 16> &get_data() const { return data; };

private:
    std::array<double, 16> data;
};
