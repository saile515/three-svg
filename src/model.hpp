#pragma once
#include <string>

class Model {
public:
    Model(std::string path);

private:
    int load_model(std::string path, std::string mtl_path);
};
