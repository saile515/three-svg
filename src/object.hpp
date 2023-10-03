#pragma once

#include "matrix.hpp"
#include "model.hpp"
#include "vector.hpp"
#include <string>

class Object {
public:
    Object(Vector3 position, Vector3 rotation, Vector3 scale, Model model);
    std::string get_render_string();
    void calculate_mvp_matrix(Matrix4x4 view_matrix, Matrix4x4 projection_matrix);

private:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Matrix4x4 model_matrix;
    Matrix4x4 mvp_matrix; // Model View Projection matrix
    Model model;

    void calculate_model_matrix();
};
