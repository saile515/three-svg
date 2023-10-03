#include "matrix.hpp"
#include "vector.hpp"
#include <cmath>
#include <numbers>
#include <utility>

Matrix4x4::Matrix4x4(double data[16]) : data{*data} {}

const double &Matrix4x4::operator[](int index) const { return data[index]; };

Matrix4x4 operator*(const Matrix4x4 &a, const Matrix4x4 &b) {
    double data[16] = {
        a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12],
        a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13],
        a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14],
        a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15],
        a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12],
        a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13],
        a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14],
        a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15],
        a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12],
        a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13],
        a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14],
        a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15],
        a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12],
        a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13],
        a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14],
        a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15]};

    return Matrix4x4(data);
}

Matrix4x4 Matrix4x4::perspective(double fov, double width, double height, double near, double far) {
    double values[16] = {(1 / std::tan(fov / 2)) / (width / height), 0, 0, 0,
                         0, 1 / std::tan(fov / 2), 0, 0,
                         0, 0, (far + near) / (near - far), -1,
                         0, 0, (far * near * 2) / (near - far), 0};
    return Matrix4x4(values);
}

Matrix4x4 Matrix4x4::orthographic(double left, double right, double bottom, double top, double near, double far) {
    double values[] = {2 / (right - left), 0, 0, -((right + left) / (right - left)),
                       0, 2 / (top - bottom), 0, -((top + bottom) / (top - bottom)),
                       0, 0, (-2) / (far / near), -((far + near) / (far - near)),
                       0, 0, 0, 1};
    return Matrix4x4(values);
}

Matrix4x4 Matrix4x4::position(Vector3 vector) {
    double data[16] = {
        1, 0, 0, vector[0],
        0, 1, 0, vector[1],
        0, 0, 1, vector[2],
        0, 0, 0, 1};

    return Matrix4x4(data);
}

Matrix4x4 Matrix4x4::rotation(Vector3 vector) {
    double x = vector[0];
    double y = vector[1];
    double z = vector[2];

    double x_data[16] = {1, 0, 0, 0,
                         0, std::cos(x), -std::sin(x), 0,
                         0, std::sin(x), std::cos(x), 0,
                         0, 0, 0, 1};

    Matrix4x4 x_matrix(x_data);

    double y_data[16] = {std::cos(y), 0, std::sin(y), 0,
                         0, 1, 0, 0,
                         -std::sin(y), 0, std::cos(y), 0,
                         0, 0, 0, 1};

    Matrix4x4 y_matrix(y_data);

    double z_data[16] = {std::cos(z), -std::sin(z), 0, 0,
                         std::sin(z), std::cos(z), 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1};

    Matrix4x4 z_matrix(z_data);

    return x_matrix * y_matrix * z_matrix;
}

Matrix4x4 Matrix4x4::scale(Vector3 vector) {
    double data[16] = {
        vector[0], 0, 0, 0,
        0, vector[1], 0, 0,
        0, 0, vector[2], 0,
        0, 0, 0, 1};

    return Matrix4x4(data);
}
