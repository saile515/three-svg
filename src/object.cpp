#include "object.hpp"
#include "model.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <iomanip>
#include <sstream>
#include <iostream>

Object::Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Model model)
    : position(position), rotation(rotation), scale(scale), model(model) {}

void Object::calculate_mvp_matrix(glm::mat4 view_matrix, glm::mat4 projection_matrix) {
    calculate_model_matrix();

    mvp_matrix = projection_matrix * view_matrix * model_matrix;
}

void Object::calculate_model_matrix() {
    glm::mat4 position_matrix = glm::translate(position);
    glm::mat4 rotation_matrix = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
    glm::mat4 scale_matrix = glm::scale(scale);

    model_matrix = position_matrix * rotation_matrix * scale_matrix;
}

static glm::vec4 vertex_from_index(tinyobj::index_t index, Model &model) {
    return glm::vec4(model.vertices[size_t(index.vertex_index) * 3], 
                     model.vertices[size_t(index.vertex_index) * 3 + 1], 
                     model.vertices[size_t(index.vertex_index)*3 + 2], 1);
}

std::string Object::get_render_string(glm::vec4 camera_position) {
    std::stringstream render_string;

    for (size_t i = 0; i < model.indices.size(); i += 3) {
        glm::vec4 vertex1 = mvp_matrix * vertex_from_index(model.indices[i], model);
        glm::vec4 vertex2 = mvp_matrix * vertex_from_index(model.indices[i + 1], model);
        glm::vec4 vertex3 = mvp_matrix * vertex_from_index(model.indices[i + 2], model);

        glm::vec4 normal = glm::vec4(model.normals[size_t(model.indices[i].normal_index) * 3], model.normals[size_t(model.indices[i].normal_index) * 3 + 1], model.normals[size_t(model.indices[i].normal_index) * 3 + 2], 1);

        glm::mat4 rotation_matrix =  glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));

        if (glm::dot((model_matrix * vertex_from_index(model.indices[i], model) - camera_position), rotation_matrix * normal) >= 0) {
            continue;
        }

        render_string
            << "<polygon points=\""
            << vertex1[0] * 50 + 50 << "," << vertex1[1] * 50 + 50 << " " << vertex2[0] * 50 + 50 << "," << vertex2[1] * 50 + 50 << " " << vertex3[0] * 50 + 50 << "," << vertex3[1] * 50 + 50
            << "\" fill=\"#"
            << std::hex << 255 << std::hex << 255 << std::hex << 255
            << "\" stroke=\"#000000\" />";
    }

    return render_string.str();
}
