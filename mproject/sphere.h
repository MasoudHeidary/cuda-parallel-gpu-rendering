#pragma once
#include <glm/glm.hpp>
#include "ray.h"
#include "hit.h"

struct Sphere {
    glm::vec3 center;
    float radius;
    glm::vec3 color;

    Sphere(const glm::vec3& c, float r, const glm::vec3& col) : center(c), radius(r), color(col) {}

    bool intersect(const Ray& ray, Hit& hit) const {
        glm::vec3 oc = ray.origin - center;
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0 * glm::dot(oc, ray.direction);
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant > 0) {
            float t = (-b - sqrt(discriminant)) / (2.0 * a);
            if (t < 0) t = (-b + sqrt(discriminant)) / (2.0 * a);  // Check second solution if first is behind

            if (t >= 0) {
                hit.t = t;
                hit.color = color;
                glm::vec3 point = ray.at(t);
                hit.normal = glm::normalize(point - center);  // Normal at the intersection point
                hit.center = center;  // Store the center of the sphere
                return true;
            }
        }
        return false;
    }
};
