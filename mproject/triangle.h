#pragma once
#include <glm/glm.hpp>
#include "ray.h"
#include "hit.h"



struct Triangle {
    glm::vec3 v0, v1, v2;  // Triangle vertices
    glm::vec3 normal;      // Precomputed normal for the triangle
    glm::vec3 color;       // Color for the triangle (you can use a default value)

    Triangle(const glm::vec3& _v0, const glm::vec3& _v1, const glm::vec3& _v2, const glm::vec3& col)
        : v0(_v0), v1(_v1), v2(_v2), color(col) {
        normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));  // Precompute the normal
    }

    bool intersect(const Ray& ray, Hit& hit) const {
        const float EPSILON = 1e-8;
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 h = glm::cross(ray.direction, edge2);
        float a = glm::dot(edge1, h);
        if (fabs(a) < EPSILON) {
            return false;  // Ray is parallel to the triangle
        }
        float f = 1.0f / a;
        glm::vec3 s = ray.origin - v0;
        float u = f * glm::dot(s, h);
        if (u < 0.0 || u > 1.0) {
            return false;
        }
        glm::vec3 q = glm::cross(s, edge1);
        float v = f * glm::dot(ray.direction, q);
        if (v < 0.0 || u + v > 1.0) {
            return false;
        }
        float t = f * glm::dot(edge2, q);
        if (t > EPSILON) {  // Ray intersection
            hit.t = t;
            hit.normal = normal;
            hit.color = color;  // Set the triangle's color
            return true;
        }
        else {
            return false;  // No hit, ray is parallel to the triangle
        }
    }
};