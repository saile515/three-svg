#include "object.hpp"
#include "matrix.hpp"
#include "vector.hpp"

#include <iomanip>
#include <sstream>

Object::Object(Vector3 position, Vector3 rotation, Vector3 scale, Model model)
    : position(position), rotation(rotation), scale(scale), model(model) {}

void Object::calculate_mvp_matrix(Matrix4x4 view_matrix, Matrix4x4 projection_matrix) {
    calculate_model_matrix();

    mvp_matrix = projection_matrix * view_matrix * model_matrix;
}

void Object::calculate_model_matrix() {
    Matrix4x4 position_matrix = Matrix4x4::position(position);
    Matrix4x4 rotation_matrix = Matrix4x4::rotation(rotation);
    Matrix4x4 scale_matrix = Matrix4x4::scale(scale);

    model_matrix = position_matrix * rotation_matrix * scale_matrix;
}

static Vector4 world_space_to_screen_space(double x, double y, double z, Matrix4x4 matrix) {
    double world_space_data[3] = {x, y, z};
    Vector3 world_space(world_space_data);
    Vector4 screen_space = matrix * Vector4(world_space, 1);
    return screen_space;
}

std::string Object::get_render_string() {
    std::stringstream render_string;

    for (size_t i = 0; i < model.vertices.size(); i += 3) {
        Vector4 vertex1 = world_space_to_screen_space(model.vertices[i], model.vertices[i + 1], model.vertices[i + 2], mvp_matrix);
        Vector4 vertex2 = world_space_to_screen_space(model.vertices[i + 3], model.vertices[i + 4], model.vertices[i + 5], mvp_matrix);
        Vector4 vertex3 = world_space_to_screen_space(model.vertices[i + 6], model.vertices[i + 7], model.vertices[i + 8], mvp_matrix);
        render_string << "<polygon points=\""
                      << vertex1[0] << "," << vertex1[1] << " " << vertex2[0] << "," << vertex2[1] << " " << vertex3[0] << "," << vertex3[1]
                      << "\" style=\"fill:#"
                      << std::hex << int(model.colors[i] * 255) << std::hex << int(model.colors[i + 1] * 255) << std::hex << int(model.colors[i + 2] * 255)
                      << "\" />";
    }

    return render_string.str();
}
