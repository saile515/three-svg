#include "vector.hpp"
#include "matrix.hpp"

Vector3::Vector3(double data[4]) : data{*data} {}

const double &Vector3::operator[](int index) const { return data[index]; };

Vector3 operator*(const Matrix4x4 &a, const Vector3 &b) {
    double data[4] = {a[0] * b[0] + a[1] * b[0] + a[2] * b[0] + a[3] * b[0],
                      a[4] * b[1] + a[5] * b[1] + a[6] * b[1] + a[7] * b[1],
                      a[8] * b[2] + a[9] * b[2] + a[10] * b[2] + a[11] * b[2],
                      a[12] * b[3] + a[13] * b[3] + a[14] * b[3] +
                          a[15] * b[3]};
    return Vector3(data);
};
