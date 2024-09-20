
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
#include <string>

//#include <glm/glm.hpp>

#include <tira/image.h>
#include <tira/parser.h>

//#include "ray.h"
//#include "hit.h"
#include "loader.h"
#include "compute.h"
#include "gobject.h"
using namespace GeoShape;









int _main(void) {
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;
    std::vector<Triangle> triangles;
    std::vector<Light> lights;
    Camera camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 0.0f);
    glm::vec3 background(0.0f, 0.0f, 0.0f);


    // Load scene from file using TIRA parser
    if (!load_scene(SCENE_FILE_NAME, spheres, planes, camera, lights, background)) {
        std::cerr << "Error loading scene!\t" << SCENE_FILE_NAME << std::endl;
        return -1;
    }

    #if OBJ_FILE_NAME
    // Load the OBJ file
    if (!load_obj(OBJ_FILE_NAME, triangles, glm::vec3(1.0, 1.0, 1.0))) {
        std::cerr << "Error loading OBJ file!\t" << OBJ_FILE_NAME << std::endl;
        return -1;
    }
    #endif

    int image_width = IMAGE_WIDTH;
    int image_height = IMAGE_HEIGHT;
    //float aspect_ratio = float(image_width) / float(image_height);
    tira::image<unsigned char> image(image_width, image_height, 3);  // 3 channels (RGB)

    for (int j = 0; j < image_height; ++j) {
        std::cout << "image height " << "[" << j << "/" << image_height << "]" << std::endl;
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
                    if (hit_record.t < closest_t) {
                        closest_t = hit_record.t;
                        closest_hit_record = hit_record;
                        hit_anything = true;
                    }
                }
            }

            // Check plane intersections
            for (const Plane& plane : planes) {
                Hit hit_record;
                if (plane.intersect(ray, hit_record)) {
                    if (hit_record.t < closest_t) {
                        closest_t = hit_record.t;
                        closest_hit_record = hit_record;
                        hit_anything = true;
                    }
                }
            }

            // Check triangle intersections
            for (const Triangle& triangle : triangles) {
                Hit hit_record;
                if (triangle.intersect(ray, hit_record)) {
                    if (hit_record.t < closest_t) {
                        closest_t = hit_record.t;
                        closest_hit_record = hit_record;
                        hit_anything = true;
                    }
                }
            }

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


    std::string _file_name;
#if OUTPUT_IMAGE_NAME
    _file_name = OUTPUT_IMAGE_NAME;
#else
     _file_name = (std::string)SCENE_FILE_NAME + ".bmp";
#endif
     image.save(_file_name);

     // open the picture after saving
     system(_file_name.c_str());
    

	return 0;
}