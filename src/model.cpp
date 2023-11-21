#include "model.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_USE_DOUBLE
#include "tiny_obj_loader.h"
#include <iostream>
#include <vector>

Model::Model(std::vector<tinyobj::real_t> vertices,
             std::vector<tinyobj::real_t> colors,
             std::vector<tinyobj::real_t> normals,
             std::vector<tinyobj::index_t> indices)
  : vertices(vertices)
  , colors(colors)
  , normals(normals)
  , indices(indices) {}

std::vector<Model> Model::load_from_obj(std::string path) {
    tinyobj::ObjReaderConfig config;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(path, config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    tinyobj::attrib_t attributes = reader.GetAttrib();
    std::vector<tinyobj::shape_t> shapes = reader.GetShapes();
    std::vector<tinyobj::material_t> materials = reader.GetMaterials();

    std::vector<Model> models;

    for (tinyobj::shape_t& shape : shapes) {
        // Check for triangulation
        for (int num_face_vertices : shape.mesh.num_face_vertices) {
            if (num_face_vertices != 3) {
                std::cout << "Error: model  \"" << path
                          << "\" contains faces with more than 3 vertices.\n";
                exit(1);
            }
        }

        models.push_back(Model(attributes.vertices,
                               attributes.colors,
                               attributes.normals,
                               shape.mesh.indices));
    }

    return models;
};
