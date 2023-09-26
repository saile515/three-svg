#pragma once
#include <string>
#include <vector>

class Model {
public:
    Model(std::vector<double> vertices, std::vector<double> colors);
    [[nodiscard]] static std::vector<Model> load_from_obj(std::string path, std::string mtl_path);

private:
    std::vector<double> vertices;
    std::vector<double> colors;
};
