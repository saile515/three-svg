#include "object.hpp"
#include "parser.hpp"
#include <fstream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Error: Invalid arguments.";
        exit(1);
    }

    Parser scene_parser(argv[1]);
    scene_parser.parse();
    SceneProperties scene = scene_parser.result;

    glm::mat4 projection_matrix;
    if (scene.camera.type == "perspective") {
        projection_matrix =
            glm::perspective(glm::radians(scene.camera.fov),
                             scene.camera.width / scene.camera.height,
                             scene.camera.near,
                             scene.camera.far);
    } else if (scene.camera.type == "orthographic") {
        projection_matrix = glm::ortho(-scene.camera.width / 2,
                                       scene.camera.width / 2,
                                       scene.camera.height / 2,
                                       -scene.camera.height / 2,
                                       scene.camera.near,
                                       scene.camera.far);
    }

    glm::mat4 view_matrix =
        glm::lookAt(glm::vec3(scene.camera.position[0],
                              scene.camera.position[1],
                              scene.camera.position[2]),
                    glm::vec3(glm::radians(float(scene.camera.rotation[0])),
                              glm::radians(float(scene.camera.rotation[1])),
                              glm::radians(float(scene.camera.rotation[2]))),
                    glm::vec3(0, 1, 0));

    std::vector<Object> objects;

    for (SceneProperties::ObjectProperties& object : scene.objects) {
        std::vector<Model> models = Model::load_from_obj(object.model);
        for (Model& model : models) {
            objects.push_back(Object(
                glm::vec3(
                    object.position[0], object.position[1], object.position[2]),
                glm::vec3(
                    object.rotation[0], object.rotation[1], object.rotation[2]),
                glm::vec3(object.scale[0], object.scale[1], object.scale[2]),
                model));
        }
    }

    std::string output_string =
        "<svg viewBox=\"0 0 100 100\" xmlns=\"http://www.w3.org/2000/svg\">";

    for (Object& object : objects) {
        object.calculate_mvp_matrix(view_matrix, projection_matrix);

        output_string +=
            object.get_render_string(glm::vec4(scene.camera.position[0],
                                               scene.camera.position[1],
                                               scene.camera.position[2],
                                               1),
                                     scene.lighting);
    }

    output_string += "</svg>";

    std::ofstream output_file(argv[2]);
    output_file << output_string;
    output_file.close();

    return 0;
}
