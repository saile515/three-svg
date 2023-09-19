#include "matrix.hpp"
#include "vector.hpp"
#include <iostream>

int main(int argc, char **argv) {
    Matrix4x4 mat1 = Matrix4x4::perspective(90, 0.1, 1000);
    Matrix4x4 mat2 = Matrix4x4::orthographic(-1, 1, -1, 1, 0.1, 1000);
    Matrix4x4 mat3 = mat1 * mat2;
    std::cout << "Hello World\n";
    return 0;
}
