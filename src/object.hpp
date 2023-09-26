#pragma once

#include "matrix.hpp"
#include "vector.hpp"
#include <string>

class Object {
public:
    std::string get_render_string();

private:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Matrix4x4 model_matrix;
    Matrix4x4 mvp_matrix; // Model View Projection matrix

    void calculate_model_matrix();
};
