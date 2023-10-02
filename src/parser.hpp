#pragma once

#include <string>
#include <vector>

struct SceneProperties {
    std::string version;

    struct CameraProperties {
        std::string type;
        double position[3];
        double rotation[3];
        double fov;
        double near;
        double far;
    };

    CameraProperties camera;

    struct LightingProperties {
        struct AmbientLightProperties {
            double color[3];
            double intensity;
        };

        AmbientLightProperties ambient;

        struct DirectionalLightProperties {
            double color[3];
            double intensity;
            double rotation[3];
        };

        DirectionalLightProperties directional;
    };

    struct ObjectProperties {
        std::string model;
        std::string material;
        double position[3];
        double rotation[3];
        double scale[3];
    };

    std::vector<ObjectProperties> objects;
};
