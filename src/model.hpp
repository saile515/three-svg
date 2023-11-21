#pragma once

#include "tiny_obj_loader.h"
#include <string>
#include <vector>

class Model {
public:
    Model(std::vector<tinyobj::real_t> vertices, std::vector<tinyobj::real_t> colors, std::vector<tinyobj::real_t> normals,
          std::vector<tinyobj::index_t> indices);
    [[nodiscard]] static std::vector<Model> load_from_obj(std::string path, std::string mtl_path);

    std::vector<tinyobj::real_t> vertices;
    std::vector<tinyobj::real_t> colors;
    std::vector<tinyobj::real_t> normals;
    std::vector<tinyobj::index_t> indices;
};
