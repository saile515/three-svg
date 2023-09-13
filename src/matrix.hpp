#pragma once
class Vector3;
class Matrix4x4;

Matrix4x4 operator*(const Matrix4x4 &a, const Matrix4x4 &b);
Vector3 operator*(const Matrix4x4 &a, const Vector3 &b);

class Matrix4x4 {
public:
    Matrix4x4(float data[16]);
    const float &operator[](int index) const;

private:
    float data[16];
};
