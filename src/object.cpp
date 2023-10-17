#include "object.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iomanip>
#include <sstream>

Object::Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Model model)
    : position(position), rotation(rotation), scale(scale), model(model) {}

void Object::calculate_mvp_matrix(glm::mat4 view_matrix, glm::mat4 projection_matrix) {
    calculate_model_matrix();

    mvp_matrix = projection_matrix * view_matrix * model_matrix;
}

void Object::calculate_model_matrix() {
    glm::mat4 position_matrix = glm::translate(position);
    glm::mat4 rotation_matrix = glm::rotate(rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(rotation.y, glm::vec3(0, 1, 0)) * glm::rotate(rotation.z, glm::vec3(0, 0, 1));
    glm::mat4 scale_matrix = glm::scale(scale);

    model_matrix = position_matrix * rotation_matrix * scale_matrix;
}

std::string Object::get_render_string() {
    std::stringstream render_string;

    for (size_t i = 0; i < model.vertices.size(); i += 3) {
        glm::vec4 vertex1 = mvp_matrix * glm::vec4(model.vertices[i], model.vertices[i + 1], model.vertices[i + 2], 1);
        glm::vec4 vertex2 = mvp_matrix * glm::vec4(model.vertices[i + 3], model.vertices[i + 4], model.vertices[i + 5], 1);
        glm::vec4 vertex3 = mvp_matrix * glm::vec4(model.vertices[i + 6], model.vertices[i + 7], model.vertices[i + 8], 1);
        model.colors[i] = 0.5;
        render_string << "<polygon points=\""
                      << vertex1[0] * 50 + 50 << "," << vertex1[1] * 50 + 50 << " " << vertex2[0] * 50 + 50 << "," << vertex2[1] * 50 + 50 << " " << vertex3[0] * 50 + 50 << "," << vertex3[1] * 50 + 50
                      << "\" fill=\"#"
                      << std::hex << int(model.colors[i] * 255) << std::hex << int(model.colors[i + 1] * 255) << std::hex << int(model.colors[i + 2] * 255)
                      << "\" />";
    }

    return render_string.str();
}
