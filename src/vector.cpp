#include "vector.hpp"
#include "matrix.hpp"

Vector3::Vector3(std::array<double, 3> data) : data{data} {}
Vector4::Vector4(std::array<double, 4> data) : data{data} {}
Vector4::Vector4(Vector3 vector, double w) : data{vector[0], vector[1], vector[2], w} {}

const double &Vector3::operator[](int index) const { return data[index]; };
const double &Vector4::operator[](int index) const { return data[index]; };

Vector4 operator*(const Matrix4x4 &a, const Vector4 &b) {
    std::array<double, 4> data({a[0] * b[0] + a[1] * b[0] + a[2] * b[0] + a[3] * b[0],
                                a[4] * b[1] + a[5] * b[1] + a[6] * b[1] + a[7] * b[1],
                                a[8] * b[2] + a[9] * b[2] + a[10] * b[2] + a[11] * b[2],
                                a[12] * b[3] + a[13] * b[3] + a[14] * b[3] + a[15] * b[3]});
    return Vector4(data);
};
