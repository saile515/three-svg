#pragma once

#include "model.hpp"
#include <glm/glm.hpp>
#include <string>
#include "parser.hpp"

class Object {
public:
    Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Model model);
    std::string get_render_string(glm::vec4 camera_position, SceneProperties::LightingProperties& lighting);
    void calculate_mvp_matrix(glm::mat4 view_matrix, glm::mat4 projection_matrix);

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 model_matrix;
    glm::mat4 mvp_matrix; // Model View Projection matrix
    Model model;

    void calculate_model_matrix();
};
