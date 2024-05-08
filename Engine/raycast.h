#ifndef RAYCAST_H
#define RAYCAST_H

#include <memory>
#include "Engine/ellipsecollider.h"
#include "Engine/transformcomponent.h"
#include "glm/vec3.hpp"
#include "triangle.h"

class GameObject;

class Ray {
public:
    float minT = 999999.0f;
    std::weak_ptr<GameObject> closestObj = std::make_shared<GameObject>();
    glm::vec3 p;
    glm::vec3 d;
    Ray(glm::vec3 p, glm::vec3 d) : p(p), d(d) {}
    void reset(){minT = 999999.0f; closestObj = std::make_shared<GameObject>();}
    void intersect(std::shared_ptr<EllipseCollider> ellipsoid) {
        if (ellipsoid->ignoreRaycast) return;
        // Print the contents of vector p
        std::cout << "p: (" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;

        // Print the contents of vector d
        std::cout << "d: (" << d.x << ", " << d.y << ", " << d.z << ")" << std::endl;
        float t0, t1; // Solutions for t if the ray intersects the sphere
#if 0 \
    // Geometric solution
        Vec3f L = center - ray.orig;
        float tca = L.dotProduct(ray.dir);
        // if (tca < 0) return false;
        float d2 = L.dotProduct(L) - tca * tca;
        if (d2 > radius * radius) return false;
        float thc = sqrt(radius * radius - d2);
        t0 = tca - thc;
        t1 = tca + thc;
#else \
    // Analytic solution
        glm::vec3 center = ellipsoid->m_transform->getPos();
        float radius = ellipsoid->m_transform->getScale().x / 2.0f;
        glm::vec3 L = p - center;
        float a = glm::dot(d, d);
        float b = 2 * glm::dot(d,L);
        float c = glm::dot(L, L) - radius * radius;
        if (!Triangle::solveQuadratic(a, b, c, t0, t1)) return;
#endif
        if (t0 > t1) std::swap(t0, t1);

        if (t0 < 0) {
            t0 = t1; // If t0 is negative, let's use t1 instead.
            if (t0 < 0) return; // Both t0 and t1 are negative.
        }

        if (t0 >= 0.0 && t0 < minT) {
            minT = t0;
            closestObj = ellipsoid->m_transform->getParent();
            std::cout << minT << std::endl;
            if (!closestObj.expired()) {
                std::cout << "closestObj is pointing to a valid GameObject" << std::endl;
            }
        }
    }
private:

};

#endif // RAYCAST_H
