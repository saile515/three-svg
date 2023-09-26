#include "model.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>
#include <vector>

Model::Model(std::vector<double> vertices, std::vector<double> colors) : vertices(vertices), colors(colors) {}

std::vector<Model> Model::load_from_obj(std::string path, std::string mtl_path) {
    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = mtl_path;

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

    for (tinyobj::shape_t &shape : shapes) {
        // Check for triangulation
        for (int num_face_vertices : shape.mesh.num_face_vertices) {
            if (num_face_vertices != 3) {
                std::cout << "Error: model  \"" << path << "\" contains faces with more than 3 vertices.\n";
                exit(1);
            }
        }

        std::vector<double> vertices;
        std::vector<double> colors;

        for (tinyobj::index_t index : shape.mesh.indices) {
            vertices.push_back(double(attributes.vertices[size_t(index.vertex_index)]));
            colors.push_back(double(attributes.colors[size_t(index.vertex_index)]));
        }

        models.push_back(Model(vertices, colors));
    }

    return models;
};
