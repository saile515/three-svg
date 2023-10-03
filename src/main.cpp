#include "matrix.hpp"
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
    return 0;
}
