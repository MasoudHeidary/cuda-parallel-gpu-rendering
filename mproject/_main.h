
#include <iostream>

#include "setting.h"


// tira fix bug
#include <cstddef> // for std::byte
std::byte myByte = std::byte(0xFF);
#define byte windows_byte
#include <Windows.h>
#undef byte
#include <cstddef> 

#include <iostream>
#include <vector>
#include <cstdlib>  // run command on terminal

#include <glm/glm.hpp>

#include <tira/image.h>
#include <tira/parser.h>

#include "ray.h"
#include "camera.h"
#include "hit.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"

struct Light {
    glm::vec3 position;
    glm::vec3 color;
};

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

glm::vec3 compute_lighting(const glm::vec3& point, const glm::vec3& normal, const std::vector<Light>& lights, const std::vector<Sphere>& spheres) {
    glm::vec3 light_color(0.0f);
    //glm::vec3 ambient_light(0.2f, 0.2f, 0.2f);
    //glm::vec3 light_color = ambient_light;

    for (const auto& light : lights) {
        // Check if the point is in shadow with respect to the light
        //if (is_in_shadow(point, normal, light, spheres)) {
        //    continue;  // Skip this light if the point is in shadow
        //}

        // Compute lighting as before (Lambertian shading)
        glm::vec3 light_dir = glm::normalize(light.position - point);
        float diffuse_intensity = std::max(glm::dot(normal, light_dir), 0.0f);
        light_color += diffuse_intensity * light.color;
    }

    return glm::clamp(light_color, 0.0f, 1.0f);  // Clamp the result to [0, 1] for valid colors
}

int _main(void) {
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;
    std::vector<Triangle> triangles;
    std::vector<Light> lights;
    Camera camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 0.0f);
    glm::vec3 background(0.0f, 0.0f, 0.0f);


    // Load scene from file using TIRA parser
    if (!load_scene("basic.scene", spheres, planes, camera, lights, background)) {
        std::cerr << "Error loading scene!" << std::endl;
        return -1;
    }


    std::cout << "camera on: " << camera.position.x << ":" << camera.position.y << ":" << camera.position.z << std::endl;
    //for(int i=0; i<1; i++)
    //    std::cout << "sphere ["<<i<<"]: " << 
    //    spheres[i].center.x << ":" << spheres[i].center.y << ":" << spheres[0].center.z << "\t" <<
    //    "(r):" << spheres[i].radius <<
    //    std::endl;

    int image_width = 1000;
    int image_height = 1000;
    //float aspect_ratio = float(image_width) / float(image_height);
    tira::image<unsigned char> image(image_width, image_height, 3);  // 3 channels (RGB)

    for (int j = 0; j < image_height; ++j) {
        std::cout << "image height " << "[" << j << "]" << std::endl;
        for (int i = 0; i < image_width; ++i) {
            float u = float(image_width - 1 - i) / float(image_width - 1);
            float v = float(image_height - 1 - j) / (image_height - 1);  // Flip the v coordinate

            Ray ray = camera.get_ray(u, v);
            glm::vec3 pixel_color = background;
            Hit closest_hit_record;  // Store the closest hit information
            bool hit_anything = false;
            float closest_t = std::numeric_limits<float>::max();  // Set to a very large number initially

            // Iterate through each sphere to find the closest intersection
            for (const Sphere& sphere : spheres) {
                Hit hit_record;
                if (sphere.intersect(ray, hit_record)) {
                    // Check if this hit is closer than the previously found hits
                    //hit_record.center = sphere.center;  // Store the center of the hit object
                    //glm::vec3 normal = glm::normalize(point - sphere.center);  // Normal at the sphere's surface
                    //hit_record.normal = normal;
                    if (hit_record.t < closest_t) {
                        closest_t = hit_record.t;
                        closest_hit_record = hit_record;
                        hit_anything = true;
                    }
                }
            }


            //// Compute the color based on the closest hit
            //if (hit_anything) {
            //    glm::vec3 point = ray.at(closest_hit_record.t);
            //    //glm::vec3 normal = glm::normalize(point - closest_hit_record.center);  // Calculate normal
            //    glm::vec3 normal = closest_hit_record.normal;
            //    glm::vec3 lighting = compute_lighting(point, normal, lights, spheres);
            //    pixel_color = glm::clamp(closest_hit_record.color * lighting, 0.0f, 1.0f);
            //}
            // Compute lighting and color based on the closest hit
            if (hit_anything) {
                glm::vec3 point = ray.at(closest_hit_record.t);
                glm::vec3 lighting = compute_lighting(point, closest_hit_record.normal, lights, spheres);
                pixel_color = glm::clamp(closest_hit_record.color * lighting, 0.0f, 1.0f);
            }
            else {
                pixel_color = background;  // If no intersection, use the background color
            }

            image(i, j, 0) = static_cast<unsigned char>(255.99 * pixel_color.r);
            image(i, j, 1) = static_cast<unsigned char>(255.99 * pixel_color.g);
            image(i, j, 2) = static_cast<unsigned char>(255.99 * pixel_color.b);
        }
    }

    // Save the rendered image using TIRA's image class
    image.save("output.bmp");
	

    // run command on terminal
    system("output.bmp");

	return 0;
}