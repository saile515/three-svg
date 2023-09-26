#include "object.hpp"
#include "matrix.hpp"

void Object::calculate_model_matrix() {
    Matrix4x4 position_matrix = Matrix4x4::position(position);
    Matrix4x4 rotation_matrix = Matrix4x4::rotation(rotation);
    Matrix4x4 scale_matrix = Matrix4x4::scale(scale);

    model_matrix = position_matrix * rotation_matrix * scale_matrix;
}
