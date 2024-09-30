#pragma once

#include <tira/image.h>
#include <tira/parser.h>

#include "gobject.h"
using namespace GeoShape;

// Function to load scene using TIRA's parser
bool load_scene(const std::string& scene_file, std::vector<Sphere>& spheres, std::vector<Plane>& planes, Camera& camera, std::vector<Light>& lights, glm::vec3& background) {
    tira::parser scene_parser(scene_file);

    // Load camera settings
    camera.position = glm::vec3(scene_parser.get<float>("camera_position", 0, 0),
        scene_parser.get<float>("camera_position", 0, 1),
        scene_parser.get<float>("camera_position", 0, 2));

    camera.look = glm::vec3(scene_parser.get<float>("camera_look", 0, 0),
        scene_parser.get<float>("camera_look", 0, 1),
        scene_parser.get<float>("camera_look", 0, 2));

    camera.up = glm::vec3(scene_parser.get<float>("camera_up", 0, 0),
        scene_parser.get<float>("camera_up", 0, 1),
        scene_parser.get<float>("camera_up", 0, 2));

    camera.fov = scene_parser.get<float>("camera_fov", 0);

    // Load spheres
    size_t num_spheres = scene_parser.count("sphere");
    for (size_t i = 0; i < num_spheres; ++i) {
        float radius = scene_parser.get<float>("sphere", i, 0);
        float x = scene_parser.get<float>("sphere", i, 1);
        float y = scene_parser.get<float>("sphere", i, 2);
        float z = scene_parser.get<float>("sphere", i, 3);
        float r = scene_parser.get<float>("sphere", i, 4);
        float g = scene_parser.get<float>("sphere", i, 5);
        float b = scene_parser.get<float>("sphere", i, 6);

        spheres.push_back(Sphere(glm::vec3(x, y, z), radius, glm::vec3(r, g, b)));
    }

    // Load lights
    size_t num_lights = scene_parser.count("light");
    for (size_t i = 0; i < num_lights; ++i) {
        float x = scene_parser.get<float>("light", i, 0);
        float y = scene_parser.get<float>("light", i, 1);
        float z = scene_parser.get<float>("light", i, 2);
        float r = scene_parser.get<float>("light", i, 3);
        float g = scene_parser.get<float>("light", i, 4);
        float b = scene_parser.get<float>("light", i, 5);
        lights.push_back(Light{ glm::vec3(x, y, z), glm::vec3(r, g, b) });
    }

    // Load background color
    background = glm::vec3(scene_parser.get<float>("background", 0, 0),
        scene_parser.get<float>("background", 0, 1),
        scene_parser.get<float>("background", 0, 2));


    // Load planes
    size_t num_planes = scene_parser.count("plane");
    for (size_t i = 0; i < num_planes; ++i) {
        float px = scene_parser.get<float>("plane", i, 0);
        float py = scene_parser.get<float>("plane", i, 1);
        float pz = scene_parser.get<float>("plane", i, 2);
        glm::vec3 point(px, py, pz);

        float nx = scene_parser.get<float>("plane", i, 3);
        float ny = scene_parser.get<float>("plane", i, 4);
        float nz = scene_parser.get<float>("plane", i, 5);
        glm::vec3 normal(nx, ny, nz);

        float r = scene_parser.get<float>("plane", i, 6);
        float g = scene_parser.get<float>("plane", i, 7);
        float b = scene_parser.get<float>("plane", i, 8);
        glm::vec3 color(r, g, b);

        planes.push_back(Plane(point, normal, color));
    }

    return true;
}

// OBJ loader function
bool load_obj(const std::string& file_path, std::vector<Triangle>& triangles, const glm::vec3& color) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << file_path << std::endl;
        return false;
    }

    std::vector<glm::vec3> vertices;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") {
            // Parse vertex
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (prefix == "f") {
            // Parse face
            int idx0, idx1, idx2;
            iss >> idx0 >> idx1 >> idx2;
            // OBJ file indices are 1-based, so we need to subtract 1 for 0-based indexing
            triangles.push_back(Triangle(vertices[idx0 - 1], vertices[idx1 - 1], vertices[idx2 - 1], color));
        }
        //else if (prefix == "f") {
        //    // Parse face with support for "//" (ignoring normals)
        //    int idx0, idx1, idx2;
        //    char dummy;  // For ignoring slashes and normal indices
        //    iss >> idx0 >> dummy >> dummy >> std::ws;
        //    iss >> idx1 >> dummy >> dummy >> std::ws;
        //    iss >> idx2 >> dummy >> dummy >> std::ws;

        //    // OBJ file indices are 1-based, so we need to subtract 1 for 0-based indexing
        //    triangles.push_back(Triangle(vertices[idx0 - 1], vertices[idx1 - 1], vertices[idx2 - 1], color));
        //}
    }
    return true;
}


