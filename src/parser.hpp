#pragma once

#include <array>
#include <string>
#include <vector>

struct SceneProperties {
    SceneProperties();
    std::string version;

    struct CameraProperties {
        std::string type;
        std::array<double, 3> &position;
        std::array<double, 3> &rotation;
        double fov;
        double near;
        double far;
    };

    CameraProperties camera;

    struct LightingProperties {
        struct AmbientLightProperties {
            std::array<double, 3> &color;
            double intensity;
        };

        AmbientLightProperties ambient;

        struct DirectionalLightProperties {
            std::array<double, 3> &color;
            double intensity;
            std::array<double, 3> &rotation;
        };

        DirectionalLightProperties directional;
    };

    LightingProperties lighting;

    struct ObjectProperties {
        ObjectProperties();
        std::string model;
        std::string material;
        std::array<double, 3> &position;
        std::array<double, 3> &rotation;
        std::array<double, 3> &scale;
    };

    std::vector<ObjectProperties> objects;
};

class Parser {
public:
    Parser(std::string path);
    SceneProperties result;

private:
    std::string path;
    void parse();
};
