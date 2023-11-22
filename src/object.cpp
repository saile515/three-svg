#include "object.hpp"
#include "model.hpp"
#include "parser.hpp"

#include <algorithm>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>

Object::Object(glm::vec3 position,
               glm::vec3 rotation,
               glm::vec3 scale,
               Model model)
  : position(position)
  , rotation(rotation)
  , scale(scale)
  , model(model) {}

void Object::calculate_mvp_matrix(glm::mat4 view_matrix,
                                  glm::mat4 projection_matrix) {
    calculate_model_matrix();

    mvp_matrix = projection_matrix * view_matrix * model_matrix;
}

void Object::calculate_model_matrix() {
    glm::mat4 position_matrix = glm::translate(position);
    glm::mat4 rotation_matrix = glm::eulerAngleYXZ(glm::radians(rotation.y),
                                                   glm::radians(rotation.x),
                                                   glm::radians(rotation.z));
    glm::mat4 scale_matrix = glm::scale(scale);

    model_matrix = position_matrix * rotation_matrix * scale_matrix;
}

static glm::vec4 vertex_from_index(tinyobj::index_t index, Model& model) {
    return glm::vec4(model.vertices[size_t(index.vertex_index) * 3],
                     model.vertices[size_t(index.vertex_index) * 3 + 1],
                     model.vertices[size_t(index.vertex_index) * 3 + 2],
                     1);
}

static std::string vertex_to_string(glm::vec4 vertex, int width, int height) {
    return std::to_string((
               (double(vertex[0] / vertex[2]) * double(width) + double(width)) /
               2)) +
           "," +
           std::to_string(((double(vertex[1] / vertex[2]) * double(height) +
                            double(height)) /
                           2));
}

static int get_vertex_color(double directional_color,
                            double directional_intensity,
                            double ambient_color,
                            double ambient_intensity,
                            double vertex_color) {

    // (Directional + Ambient) * Vertex Color (0-255)
    return glm::clamp<int>(
        int(std::floor(((directional_color * directional_intensity +
                         ambient_color * ambient_intensity) *
                        vertex_color) *
                       255)),
        0,
        255);
}

std::string Object::get_render_string(
    glm::vec4 camera_position,
    SceneProperties::LightingProperties& lighting) {
    std::stringstream render_string;

    auto indices = model.indices;

    for (size_t i = 0; i < indices.size(); i += 3) {
        glm::vec4 a_vertex1 = vertex_from_index(indices[i], model);
        glm::vec4 a_vertex2 = vertex_from_index(indices[i + 1], model);
        glm::vec4 a_vertex3 = vertex_from_index(indices[i + 2], model);

        glm::vec4 a_center = float(1 / 3) * (a_vertex1 + a_vertex2 + a_vertex3);
        int a_distance = (camera_position - a_center).length();

        glm::vec4 b_vertex1 = vertex_from_index(indices[i + 3], model);
        glm::vec4 b_vertex2 = vertex_from_index(indices[i + 4], model);
        glm::vec4 b_vertex3 = vertex_from_index(indices[i + 5], model);

        glm::vec4 b_center = float(1 / 3) * (b_vertex1 + b_vertex2 + b_vertex3);
        int b_distance = (camera_position - b_center).length();
    }

    for (size_t i = 0; i < model.indices.size(); i += 3) {
        glm::vec4 vertex1 =
            mvp_matrix * vertex_from_index(model.indices[i], model);
        glm::vec4 vertex2 =
            mvp_matrix * vertex_from_index(model.indices[i + 1], model);
        glm::vec4 vertex3 =
            mvp_matrix * vertex_from_index(model.indices[i + 2], model);

        glm::vec4 local_normal = glm::vec4(
            model.normals[size_t(model.indices[i].normal_index) * 3],
            model.normals[size_t(model.indices[i].normal_index) * 3 + 1],
            model.normals[size_t(model.indices[i].normal_index) * 3 + 2],
            0);

        glm::mat4 rotation_matrix =
            glm::eulerAngleYXZ(glm::radians(rotation.y),
                               glm::radians(rotation.x),
                               glm::radians(rotation.z));

        glm::vec4 normal = rotation_matrix * local_normal;

        // Back-face culling
        if (glm::dot(
                (model_matrix * vertex_from_index(model.indices[i], model) -
                 camera_position),
                normal) >= 0) {
            continue;
        }

        // Lighting

        double directional_intensity = std::max<double>(
            std::min<double>(
                double(glm::dot(
                    normal,
                    glm::vec4(glm::radians(lighting.directional.rotation[0]),
                              glm::radians(lighting.directional.rotation[1]),
                              glm::radians(lighting.directional.rotation[2]),
                              0))) *
                    lighting.directional.intensity,
                1),
            0);

        int red = get_vertex_color(
            lighting.directional.color[0],
            directional_intensity,
            lighting.ambient.color[0],
            lighting.ambient.intensity,
            double(model.colors[size_t(model.indices[i].vertex_index) * 3]));

        int green = get_vertex_color(
            lighting.directional.color[1],
            directional_intensity,
            lighting.ambient.color[1],
            lighting.ambient.intensity,
            double(
                model.colors[size_t(model.indices[i].vertex_index) * 3 + 1]));

        int blue = get_vertex_color(
            lighting.directional.color[2],
            directional_intensity,
            lighting.ambient.color[2],
            lighting.ambient.intensity,
            double(
                model.colors[size_t(model.indices[i].vertex_index) * 3 + 2]));

        render_string << "<polygon points=\""
                      << vertex_to_string(vertex1, 100, 100) << " "
                      << vertex_to_string(vertex2, 100, 100) << " "
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
