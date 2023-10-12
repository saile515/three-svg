#include "matrix.hpp"
#include "vector.hpp"
#include <cmath>
#include <iostream>
#include <numbers>
#include <utility>

Matrix4x4::Matrix4x4(std::array<double, 16> data) : data{data} {}

const double &Matrix4x4::operator[](int index) const { return data[index]; };

void Matrix4x4::print() const {
    std::cout
        << "| " << data[0] << "\t | " << data[1] << "\t | " << data[2] << "\t | " << data[3] << "\t |\n"
        << "| " << data[4] << "\t | " << data[5] << "\t | " << data[6] << "\t | " << data[7] << "\t |\n"
        << "| " << data[8] << "\t | " << data[9] << "\t | " << data[10] << "\t | " << data[11] << "\t |\n"
        << "| " << data[12] << "\t | " << data[13] << "\t | " << data[14] << "\t | " << data[15] << "\t |\n\n";
}

Matrix4x4 Matrix4x4::inverse() {
    double determinant = data[0] * data[5] * data[10] * data[15] + data[0] * data[6] * data[11] * data[13] + data[0] * data[7] * data[9] * data[14]   //
                         - data[0] * data[7] * data[10] * data[13] - data[0] * data[6] * data[9] * data[15] - data[0] * data[5] * data[11] * data[14] //
                         - data[1] * data[4] * data[10] * data[15] - data[2] * data[4] * data[11] * data[13] - data[3] * data[4] * data[9] * data[14] //
                         + data[3] * data[4] * data[10] * data[13] + data[2] * data[4] * data[9] * data[15] + data[1] * data[4] * data[11] * data[14] //
                         + data[1] * data[6] * data[8] * data[15] + data[2] * data[7] * data[8] * data[13] + data[3] * data[5] * data[8] * data[14]   //
                         - data[3] * data[6] * data[8] * data[13] - data[2] * data[5] * data[8] * data[15] - data[1] * data[7] * data[8] * data[14]   //
                         - data[1] * data[6] * data[11] * data[12] - data[2] * data[7] * data[9] * data[12] - data[3] * data[5] * data[10] * data[12] //
                         + data[3] * data[6] * data[9] * data[12] + data[2] * data[5] * data[11] * data[12] + data[1] * data[7] * data[10] * data[12];

    std::array<double, 16> adjugate_data = {
        data[5] * data[10] * data[15] + data[6] * data[11] * data[13] + data[7] * data[9] * data[14] - data[7] * data[10] * data[13] - data[6] * data[9] * data[15] - data[5] * data[11] * data[14],
        -data[1] * data[10] * data[15] - data[2] * data[11] * data[13] - data[3] * data[9] * data[14] + data[3] * data[10] * data[13] + data[2] * data[9] * data[15] + data[1] * data[11] * data[14],
        data[1] * data[6] * data[15] + data[2] * data[7] * data[13] + data[3] * data[5] * data[14] - data[3] * data[6] * data[13] - data[2] * data[5] * data[15] - data[1] * data[7] * data[14],
        -data[1] * data[6] * data[11] - data[2] * data[7] * data[9] - data[3] * data[5] * data[10] + data[3] * data[6] * data[9] + data[2] * data[5] * data[11] + data[1] * data[7] * data[10],
        -data[4] * data[10] * data[15] - data[6] * data[11] * data[12] - data[7] * data[8] * data[14] + data[7] * data[10] * data[12] + data[6] * data[8] * data[15] + data[4] * data[11] * data[14],
        data[0] * data[10] * data[15] + data[2] * data[11] * data[12] + data[3] * data[8] * data[14] - data[3] * data[10] * data[12] - data[2] * data[8] * data[15] - data[0] * data[11] * data[14],
        -data[0] * data[6] * data[15] - data[2] * data[7] * data[12] - data[3] * data[4] * data[14] + data[3] * data[6] * data[12] + data[2] * data[4] * data[15] + data[0] * data[7] * data[14],
        data[0] * data[6] * data[11] + data[2] * data[7] * data[8] + data[3] * data[4] * data[10] - data[3] * data[6] * data[8] - data[2] * data[4] * data[11] - data[0] * data[7] * data[10],
        data[4] * data[9] * data[15] + data[5] * data[11] * data[12] + data[7] * data[8] * data[13] - data[7] * data[9] * data[12] - data[5] * data[8] * data[15] - data[4] * data[11] * data[13],
        -data[0] * data[9] * data[15] - data[1] * data[11] * data[12] - data[3] * data[8] * data[13] + data[3] * data[9] * data[12] + data[1] * data[8] * data[15] + data[0] * data[11] * data[13],
        data[0] * data[5] * data[15] + data[1] * data[7] * data[12] + data[3] * data[4] * data[13] - data[3] * data[5] * data[12] - data[1] * data[4] * data[15] - data[0] * data[7] * data[13],
        -data[0] * data[5] * data[11] - data[1] * data[7] * data[8] - data[3] * data[4] * data[9] + data[3] * data[5] * data[8] + data[1] * data[4] * data[11] + data[0] * data[7] * data[9],
        -data[4] * data[9] * data[14] - data[5] * data[10] * data[12] - data[6] * data[8] * data[13] + data[6] * data[9] * data[12] + data[5] * data[8] * data[14] + data[4] * data[10] * data[13],
        data[0] * data[9] * data[14] + data[1] * data[10] * data[12] + data[2] * data[8] * data[13] - data[2] * data[9] * data[12] - data[1] * data[8] * data[14] - data[0] * data[10] * data[13],
        -data[0] * data[5] * data[14] - data[1] * data[6] * data[12] - data[2] * data[4] * data[13] + data[2] * data[5] * data[12] + data[1] * data[4] * data[14] + data[0] * data[6] * data[13],
        data[0] * data[5] * data[10] + data[1] * data[6] * data[8] + data[2] * data[4] * data[9] - data[2] * data[5] * data[8] - data[1] * data[4] * data[10] - data[0] * data[6] * data[9],
    };

    Matrix4x4 adjugate(adjugate_data);

    print();

    if (determinant < 1 && determinant > -1) {
        std::cout << "Error: determinant is 0.\n";
        exit(1);
    }

    return (1 / determinant) * adjugate;
};

Matrix4x4 operator*(const Matrix4x4 &a, const Matrix4x4 &b) {
    std::array<double, 16> data = {
        a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12],
        a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13],
        a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14],
        a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15],
        a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12],
        a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13],
        a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14],
        a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15],
        a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12],
        a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14],
        a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15],
        a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12],
        a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13],
        a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14],
        a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15]};

    return Matrix4x4(data);
}

Matrix4x4 operator*(double a, const Matrix4x4 &b) {
    std::array<double, 16> data;

    for (size_t i = 0; i < 16; i++) {
        data[i] = b.get_data()[i] * a;
    }

    return Matrix4x4(data);
}

Matrix4x4 Matrix4x4::perspective(double fov, double width, double height, double near, double far) {
    std::array<double, 16> values = {(1 / std::tan(fov / 2)) / (width / height), 0, 0, 0,
                                     0, 1 / std::tan(fov / 2), 0, 0,
                                     0, 0, (far + near) / (near - far), -1,
                                     0, 0, (far * near * 2) / (near - far), 0};
    return Matrix4x4(values);
}

Matrix4x4 Matrix4x4::orthographic(double left, double right, double bottom, double top, double near, double far) {
    std::array<double, 16> values = {2 / (right - left), 0, 0, -((right + left) / (right - left)),
                                     0, 2 / (top - bottom), 0, -((top + bottom) / (top - bottom)),
                                     0, 0, (-2) / (far / near), -((far + near) / (far - near)),
                                     0, 0, 0, 1};
    return Matrix4x4(values);
}

Matrix4x4 Matrix4x4::position(Vector3 vector) {
    std::array<double, 16> data = {
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

    std::array<double, 16> x_data = {1, 0, 0, 0,
                                     0, std::cos(x), -std::sin(x), 0,
                                     0, std::sin(x), std::cos(x), 0,
                                     0, 0, 0, 1};

    Matrix4x4 x_matrix(x_data);

    std::array<double, 16> y_data = {std::cos(y), 0, std::sin(y), 0,
                                     0, 1, 0, 0,
                                     -std::sin(y), 0, std::cos(y), 0,
                                     0, 0, 0, 1};

    Matrix4x4 y_matrix(y_data);

    std::array<double, 16> z_data = {std::cos(z), -std::sin(z), 0, 0,
                                     std::sin(z), std::cos(z), 0, 0,
                                     0, 0, 1, 0,
                                     0, 0, 0, 1};

    Matrix4x4 z_matrix(z_data);

    std::cout << "Rotation matrices\n";
    x_matrix.print();
    y_matrix.print();
    z_matrix.print();
    std::cout << "_________________________________\n";

    return x_matrix * y_matrix * z_matrix;
}

Matrix4x4 Matrix4x4::scale(Vector3 vector) {
    std::array<double, 16> data = {
        vector[0], 0, 0, 0,
        0, vector[1], 0, 0,
        0, 0, vector[2], 0,
        0, 0, 0, 1};

    return Matrix4x4(data);
}
