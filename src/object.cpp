#include "object.hpp"
#include "model.hpp"
#include "parser.hpp"

#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>

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
    return glm::vec4(model.vertices[size_t(index.vertex_index) * 3], model.vertices[size_t(index.vertex_index) * 3 + 1],
                     model.vertices[size_t(index.vertex_index) * 3 + 2], 1);
}

static std::string vertex_to_string(glm::vec4 vertex, int width, int height) {
    return std::to_string((((vertex[0] / vertex[2]) * width + width) / 2)) + "," +
           std::to_string((((vertex[1] / vertex[2]) * height + height) / 2));
}

std::string Object::get_render_string(glm::vec4 camera_position, SceneProperties::LightingProperties &lighting) {
    std::stringstream render_string;

    for (size_t i = 0; i < model.indices.size(); i += 3) {
        glm::vec4 vertex1 = mvp_matrix * vertex_from_index(model.indices[i], model);
        glm::vec4 vertex2 = mvp_matrix * vertex_from_index(model.indices[i + 1], model);
        glm::vec4 vertex3 = mvp_matrix * vertex_from_index(model.indices[i + 2], model);

        std::cout << vertex1[0] << ", " << vertex1[1] << ", " << vertex1[2] << ", " << vertex1[3] << "\n";

        glm::vec4 local_normal = glm::vec4(model.normals[size_t(model.indices[i].normal_index) * 3],
                                           model.normals[size_t(model.indices[i].normal_index) * 3 + 1],
                                           model.normals[size_t(model.indices[i].normal_index) * 3 + 2], 0);

        glm::mat4 rotation_matrix = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));

        glm::vec4 normal = rotation_matrix * local_normal;

        // Back-face culling
        if (glm::dot((model_matrix * vertex_from_index(model.indices[i], model) - camera_position), normal) >= 0) {
            continue;
        }

        // Lighting

        double directional_intensity =
            std::max<double>(std::min<double>(glm::dot(normal, glm::vec4(glm::radians(lighting.directional.rotation[0]),
                                                                         glm::radians(lighting.directional.rotation[1]),
                                                                         glm::radians(lighting.directional.rotation[2]), 0)) *
                                                  lighting.directional.intensity,
                                              1),
                             0);

        int red = glm::clamp<double>(
            std::floor((lighting.directional.color[0] * directional_intensity + lighting.ambient.color[0] * lighting.ambient.intensity) *
                       255),
            0, 255);
        int green = glm::clamp<double>(
            std::floor((lighting.directional.color[1] * directional_intensity + lighting.ambient.color[1] * lighting.ambient.intensity) *
                       255),
            0, 255);
        int blue = glm::clamp<double>(
            std::floor((lighting.directional.color[2] * directional_intensity + lighting.ambient.color[2] * lighting.ambient.intensity) *
                       255),
            0, 255);

        render_string << "<polygon points=\"" << vertex_to_string(vertex1, 100, 100) << " " << vertex_to_string(vertex2, 100, 100) << " "
                      << vertex_to_string(vertex3, 100, 100) << "\" fill=\"#";

        if (red < 16) {
            render_string << "0";
        }

        render_string << std::hex << red;

        if (green < 16) {
            render_string << "0";
        }

        render_string << std::hex << green;

        if (blue < 16) {
            render_string << "0";
        }

        render_string << std::hex << blue;

        render_string << "\"/>";
    }

    return render_string.str();
}
