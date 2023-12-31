#pragma once

#include <array>
#include <string>
#include <vector>

struct SceneProperties {
    std::string version;

    struct CameraProperties {
        std::string type;
        std::array<double, 3> position;
        std::array<double, 3> look_at;
        double fov;
        double width;
        double height;
        double near;
        double far;
    };

    CameraProperties camera;

    struct LightingProperties {
        struct AmbientLightProperties {
            std::array<double, 3> color;
            double intensity;
        };

        AmbientLightProperties ambient;

        struct DirectionalLightProperties {
            std::array<double, 3> color;
            double intensity;
            std::array<double, 3> rotation;
        };

        DirectionalLightProperties directional;
    };

    LightingProperties lighting;

    struct ObjectProperties {
        std::string model;
        std::array<double, 3> position;
        std::array<double, 3> rotation;
        std::array<double, 3> scale;
    };

    std::vector<ObjectProperties> objects;
};

class Parser {
public:
    Parser(std::string path);
    SceneProperties result;
    void parse();

private:
    std::string path;
};
