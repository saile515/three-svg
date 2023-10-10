#include "matrix.hpp"
#include "object.hpp"
#include "parser.hpp"
#include "vector.hpp"
#include <iostream>

int main(int argc, char **argv) {
    Parser scene_parser(argv[1]);
    scene_parser.parse();
    SceneProperties scene = scene_parser.result;

    Matrix4x4 projection_matrix;
    if (scene.camera.type == "perspective") {
        projection_matrix = Matrix4x4::perspective(scene.camera.fov, scene.camera.width, scene.camera.height, scene.camera.near, scene.camera.far);
    } else if (scene.camera.type == "orthographic") {
        projection_matrix = Matrix4x4::orthographic(0, scene.camera.width, scene.camera.height, 0, scene.camera.near, scene.camera.far);
    }

    Matrix4x4 view_matrix = (Matrix4x4::position(Vector3(scene.camera.position)) * Matrix4x4::rotation(Vector3(scene.camera.rotation))).inverse();

    std::vector<Object> objects;

    for (SceneProperties::ObjectProperties &object : scene.objects) {
        std::vector<Model> models = Model::load_from_obj(object.model, object.material);
        for (Model &model : models) {
            objects.push_back(Object(object.position, object.rotation, object.scale, model));
        }
    }

    std::string output_string = "<svg width=\"100\" height=\"100\">";

    for (Object &object : objects) {
        object.calculate_mvp_matrix(view_matrix, projection_matrix);

        output_string += object.get_render_string();
    }

    output_string += "</svg>";

    std::cout << output_string << "\n";

    return 0;
}
