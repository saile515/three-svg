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
#include <limits>
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
    std::stringstream stream;

    stream
        << std::fixed << std::setprecision(2)
        << ((double(vertex[0] / vertex[2]) * double(width) + double(width)) / 2)
        << " " << std::fixed << std::setprecision(2)
        << ((double(vertex[1] / vertex[2]) * double(height) + double(height)) /
            2);

    return stream.str();
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

// Slightly modified version of
// https://stackoverflow.com/questions/45447361/how-to-move-certain-elements-of-stdvector-to-a-new-index-within-the-vector
template<typename T>
void move_element_in_vector(std::vector<T>& vector,
                            size_t old_index,
                            size_t new_index) {
    if (old_index > new_index)
        std::rotate(vector.rend() - old_index - 1,
                    vector.rend() - old_index,
                    vector.rend() - new_index);
    else
        std::rotate(vector.begin() + old_index,
                    vector.begin() + old_index + 1,
                    vector.begin() + new_index + 1);
}

std::string Object::get_render_string(
    glm::vec4 camera_position,
    SceneProperties::LightingProperties& lighting) {
    std::stringstream render_string;

    auto indices = model.indices;

    // Sort indices based on distance from camera in descending order
    for (size_t iteration = 0; iteration < indices.size() / 3; iteration++) {
        for (size_t i = 0; i < indices.size() - 3 - iteration * 3; i += 3) {
            glm::vec4 a[3] = {
                model_matrix * vertex_from_index(indices[i], model),
                model_matrix * vertex_from_index(indices[i + 1], model),
                model_matrix * vertex_from_index(indices[i + 2], model)
            };

            // glm::vec4 a_center = (a_vertex1 + a_vertex2 + a_vertex3);
            // float a_distance =
            //    glm::length(glm::vec3(camera_position) - glm::vec3(a_center));

            glm::vec4 b[3] = {
                model_matrix * vertex_from_index(indices[i + 3], model),
                model_matrix * vertex_from_index(indices[i + 4], model),
                model_matrix * vertex_from_index(indices[i + 5], model)
            };

            // glm::vec4 b_center = (b_vertex1 + b_vertex2 + b_vertex3);
            // float b_distance =
            //     glm::length(glm::vec3(camera_position) -
            //     glm::vec3(b_center));

            int a_min;
            int b_min;

            float min_distance = std::numeric_limits<float>::max();

            // Get shortest distance between vertices
            for (int ai = 0; ai < 3; ai++) {
                for (int bi = 0; bi < 3; bi++) {
                    if (std::abs(
                            glm::length(glm::vec3(a[ai]) - glm::vec3(b[bi]))) <
                        min_distance) {
                        a_min = ai;
                        b_min = bi;
                    }
                }
            }

            float a_distance =
                glm::length(glm::vec3(camera_position) - glm::vec3(a[a_min]));

            float b_distance =
                glm::length(glm::vec3(camera_position) - glm::vec3(b[b_min]));

            // Switch a and b if b is further away
            if (b_distance > a_distance) {
                move_element_in_vector(indices, i + 3, i);
                move_element_in_vector(indices, i + 4, i + 1);
                move_element_in_vector(indices, i + 5, i + 2);
            }
        }
    }

    for (size_t i = 0; i < indices.size(); i += 3) {
        glm::vec4 vertex1 = mvp_matrix * vertex_from_index(indices[i], model);
        glm::vec4 vertex2 =
            mvp_matrix * vertex_from_index(indices[i + 1], model);
        glm::vec4 vertex3 =
            mvp_matrix * vertex_from_index(indices[i + 2], model);

        glm::vec4 local_normal =
            glm::vec4(model.normals[size_t(indices[i].normal_index) * 3],
                      model.normals[size_t(indices[i].normal_index) * 3 + 1],
                      model.normals[size_t(indices[i].normal_index) * 3 + 2],
                      0);

        glm::vec4 normal = glm::normalize(model_matrix * local_normal);

        // Back-face culling
        if (glm::dot(model_matrix * vertex_from_index(indices[i], model) -
                         camera_position,
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
            double(model.colors[size_t(indices[i].vertex_index) * 3]));

        int green = get_vertex_color(
            lighting.directional.color[1],
            directional_intensity,
            lighting.ambient.color[1],
            lighting.ambient.intensity,
            double(model.colors[size_t(indices[i].vertex_index) * 3 + 1]));

        int blue = get_vertex_color(
            lighting.directional.color[2],
            directional_intensity,
            lighting.ambient.color[2],
            lighting.ambient.intensity,
            double(model.colors[size_t(indices[i].vertex_index) * 3 + 2]));

        std::stringstream color;

        // Add a prefixing 0 if value is less than 16, in order to always
        // represent value with two hex characters
        if (red < 16) {
            color << "0";
        }

        color << std::hex << red;

        // ^^^
        if (green < 16) {
            color << "0";
        }

        color << std::hex << green;

        // ^^^
        if (blue < 16) {
            color << "0";
        }

        color << std::hex << blue;

        // Add triangle with previously calculated coordinates
        render_string << "<path d=\"M" << vertex_to_string(vertex1, 100, 100)
                      << " L" << vertex_to_string(vertex2, 100, 100) << " L"
                      << vertex_to_string(vertex3, 100, 100) << " z\" fill=\"#"
                      << color.str();

        render_string << "\"/>";
    }

    return render_string.str();
}
