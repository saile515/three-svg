#include "parser.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <type_traits>

Parser::Parser(std::string path)
  : path(path) {}

template<typename T>
static T get_value(Json::Value parent, std::string member) {
    if (!parent[member]) {
        std::cout << "Error: " << member << " does not exist.\n";
        exit(1);
    };

    Json::Value value = parent[member];

    if constexpr (std::is_same<T, std::string>::value) {
        if (value.isString()) {
            return value.asString();
        } else {
            std::cout << "Error: " << member << " is not a string.\n";
            exit(1);
        }
    } else if constexpr (std::is_same<T, double>::value) {
        if (value.isDouble()) {
            return value.asDouble();
        } else {
            std::cout << "Error: " << member
                      << " is not a floating point number.\n";
            exit(1);
        }
    } else if constexpr (std::is_same<T, std::array<double, 3>>::value) {
        if (value.isArray()) {
            if (value.size() != 3 ||
                (!value[0].isDouble() && !value[1].isDouble() &&
                 !value[2].isDouble())) {
                std::cout << "Error: " << member
                          << " does not contain 3 floating point numbers.\n";
                exit(1);
            }

            std::array<double, 3> arr = { value[0].asDouble(),
                                          value[1].asDouble(),
                                          value[2].asDouble() };
            return arr;
        } else {
            std::cout << "Error: " << member
                      << " is not a floating point number.\n";
            exit(1);
        }
    }
}

void Parser::parse() {
    if (!std::filesystem::exists(path)) {
        std::cout << "Error: input file does now exist.\n";
        exit(1);
    }
    std::ifstream source(path);

    Json::Value root;
    source >> root;

    result.version = get_value<std::string>(root, "version");

    if (result.version == "1.0.0") {
        // Camera
        Json::Value camera = root["camera"];
        if (camera && camera.isObject()) {
            result.camera.type = get_value<std::string>(camera, "type");
            result.camera.position =
                get_value<std::array<double, 3>>(camera, "position");
            result.camera.rotation =
                get_value<std::array<double, 3>>(camera, "rotation");
            if (result.camera.type == "perspective") {
                result.camera.fov = get_value<double>(camera, "fov");
            }
            result.camera.width = get_value<double>(camera, "width");
            result.camera.height = get_value<double>(camera, "height");
            result.camera.near = get_value<double>(camera, "near");
            result.camera.far = get_value<double>(camera, "far");
        }

        // Lighting
        Json::Value lighting = root["lighting"];
        if (lighting && lighting.isObject()) {
            // Ambient
            Json::Value ambient = lighting["ambient"];
            if (ambient && ambient.isObject()) {
                result.lighting.ambient.color =
                    get_value<std::array<double, 3>>(ambient, "color");
                result.lighting.ambient.intensity =
                    get_value<double>(ambient, "intensity");
            }

            // Directional
            Json::Value directional = lighting["directional"];
            if (directional && directional.isObject()) {
                result.lighting.directional.rotation =
                    get_value<std::array<double, 3>>(directional, "rotation");
                result.lighting.directional.color =
                    get_value<std::array<double, 3>>(directional, "color");
                result.lighting.directional.intensity =
                    get_value<double>(directional, "intensity");
            }
        }

        // Objects
        Json::Value objects = root["objects"];
        if (objects && objects.isArray()) {
            for (Json::Value& object : objects) {
                SceneProperties::ObjectProperties object_properties;
                if (object.isObject()) {
                    object_properties.model =
                        get_value<std::string>(object, "model");
                    object_properties.material =
                        get_value<std::string>(object, "material");
                    object_properties.position =
                        get_value<std::array<double, 3>>(object, "position");
                    object_properties.rotation =
                        get_value<std::array<double, 3>>(object, "rotation");
                    object_properties.scale =
                        get_value<std::array<double, 3>>(object, "scale");
                }
                result.objects.push_back(object_properties);
            }
        }
    } // TODO: implement future versions
}
