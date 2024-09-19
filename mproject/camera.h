#pragma once
#include <glm/glm.hpp>
#include "ray.h"

class Camera {
public:
    glm::vec3 position;
    glm::vec3 look;
    glm::vec3 up;
    float fov;

    Camera(const glm::vec3& pos, const glm::vec3& look_dir, const glm::vec3& up_dir, float field_of_view)
        : position(pos), look(glm::normalize(look_dir - pos)), up(glm::normalize(up_dir)), fov(field_of_view) {}

    Ray get_ray(float u, float v) const {
        float aspect_ratio = 1.0f; // Adjust this as needed
        float theta = glm::radians(fov);
        float h = glm::tan(theta / 2.0f);
        float viewport_height = 2.0f * h;
        float viewport_width = aspect_ratio * viewport_height;

        glm::vec3 w = glm::normalize(position - look);
        glm::vec3 u_vec = glm::normalize(glm::cross(up, w));
        glm::vec3 v_vec = glm::cross(w, u_vec);

        glm::vec3 lower_left = position - (viewport_width / 2.0f) * u_vec - (viewport_height / 2.0f) * v_vec - w;
        glm::vec3 horizontal = viewport_width * u_vec;
        glm::vec3 vertical = viewport_height * v_vec;

        glm::vec3 direction = glm::normalize(lower_left + u * horizontal + v * vertical - position);
        return Ray(position, direction);
    }
};
