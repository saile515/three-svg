#include "parser.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <type_traits>

Parser::Parser(std::string path) : path(path) {}

template <typename T>
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
            std::cout << "Error: " << member << " is not a floating point number.\n";
            exit(1);
        }
    } else if constexpr (std::is_same<T, double[3]>::value) {
        if (value.isArray()) {
            if (value.size() != 3 || (!value[0].isDouble() && !value[1].isDouble() && !value[2].isDouble())) {
                std::cout << "Error: " << member << " does not contain 3 floating point numbers.\n";
                exit(1);
            }

            double arr[3] = {value[0].asDouble(), value[1].asDouble(), value[2].asDouble()};
            return arr;
        } else {
            std::cout << "Error: " << member << " is not a floating point number.\n";
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
    } // TODO: implement future versions
}
