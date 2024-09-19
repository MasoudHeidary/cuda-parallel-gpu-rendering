#pragma once
#include "ray.h"
#include "hit.h"
#include <glm/glm.hpp>

class Plane {
public:
    glm::vec3 point;   // A point on the plane
    glm::vec3 normal;  // The normal vector of the plane
    glm::vec3 color;   // The color of the plane

    Plane(const glm::vec3& p, const glm::vec3& n, const glm::vec3& col)
        : point(p), normal(glm::normalize(n)), color(col) {}

    bool intersect(const Ray& ray, Hit& hit) const {
        // Compute the denominator D·N (ray direction dot plane normal)
        float denom = glm::dot(ray.direction, normal);

        // If denom is close to zero, the ray is parallel to the plane
        if (fabs(denom) > 1e-6) {  // 1e-6 is a small value to avoid precision issues
            // Compute the intersection distance (t)
            float t = glm::dot(point - ray.origin, normal) / denom;

            // Check if the intersection is in front of the ray (positive t)
            if (t >= 0) {
                hit.t = t;
                hit.color = color;
                hit.normal = normal;
                return true;
            }
        }
        return false;
    }
};
