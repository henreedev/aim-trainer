#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "glm/geometric.hpp"
#include "glm/vec3.hpp"
#include <cfloat>
#include <iostream>
#include <memory>
#include <vector>
#include "Engine/collision.h"
class Triangle {
public:
    Triangle(std::vector<glm::vec3> vertices, glm::vec3 normal, glm::vec3 centroid)
        : vertices(vertices),
        normal(normal),
        centroid(centroid)
    {}
    Triangle() {

    }
    ~Triangle(){}
    std::vector<glm::vec3> vertices;
    glm::vec3 normal;
    glm::vec3 centroid;
    std::vector<std::pair<Triangle*, glm::vec3>> neighbors;

    static bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
    {
        float discr = b * b - 4 * a * c;
        if (discr < 0) return false;
        else if (discr == 0) x0 = x1 = - 0.5 * b / a;
        else {
            float q = (b > 0) ?
                          -0.5 * (b + sqrt(discr)) :
                          -0.5 * (b - sqrt(discr));
            x0 = q / a;
            x1 = c / q;
        }
        if (x0 > x1) std::swap(x0, x1);

        return true;
    }

    Collision checkRaycast(glm::vec3 p, glm::vec3 d, bool isSphere) {
        if (isSphere) {
            p = p - normal;
        }
        float denom = glm::dot(d, normal);
        auto A = vertices[0];
        auto B = vertices[1];
        auto C = vertices[2];

        if (abs(denom) < 0.0001f) {
            // Ray is parallel to plane
            return Collision{glm::vec3(), -1, normal, normal};
        } else {
            // get plane collision
            float t = glm::dot((A - p), normal) / denom;
            // return miss if ray points wrong way or is too far
            if (t < 0 || t > 1) return Collision{glm::vec3(), -1, normal, normal};
            // check if collision point is to the left of each edge
            auto P_i = p + d * t;
            // generate edges
            auto AP = A - P_i;
            auto BP = B - P_i;
            auto CP = C - P_i;
            auto ABcross = glm::normalize(glm::cross(AP, BP));
            auto BCcross = glm::normalize(glm::cross(BP, CP));
            auto CAcross = glm::normalize(glm::cross(CP, AP));
            if (glm::dot(ABcross, normal) > 0 && glm::dot(BCcross, normal) > 0 && glm::dot(CAcross, normal) > 0 ) {
                if (isSphere) {
                    P_i = P_i + normal;
                }
                return Collision(glm::vec3(), t, normal, P_i);
            } else {
                return Collision(glm::vec3(), -1, normal, P_i);
            }
        }
    }
    Collision checkEdges(glm::vec3 p, glm::vec3 d) {
        Collision closestCol = Collision{glm::vec3(), FLT_MAX, normal, normal};
        float tMin = FLT_MAX;
        for (int i = 0; i < 3; i++) {
            int firstIndex = i;
            int secondIndex = (i + 1) % 3;
            auto E1 = vertices[firstIndex];
            auto E2 = vertices[secondIndex];
            auto edge = E2 - E1;
            auto a_sqrt = glm::cross(d , edge);
            auto a = glm::dot(a_sqrt, a_sqrt);
            auto b = glm::dot(2.0f * (glm::cross(d, edge)), (glm::cross(p - E1, edge)));
            auto c = glm::dot(glm::cross(p - E1, edge), glm::cross(p - E1, edge)) - glm::dot(edge, edge);
            float t0 = -1;
            float t1 = -1;
            float t = FLT_MAX;
            solveQuadratic(a, b, c, t0, t1);
            if (t0 > t1) std::swap(t0, t1);

            if (t0 < 0) {
                t0 = t1; // if t0 is negative, let's use t1 instead
                if (t0 < 0) continue; // both t0 and t1 are negative
            }

            t = t0;
            auto P = p + d * t;

            // CHECK IF BETWEEN C AND D
            auto prod = glm::dot((P - E1), edge);
            if (!(0 < prod && prod < glm::dot(edge, edge))) {
                continue;
            }

            Collision col = Collision{glm::vec3(), t, normal, P};
            if (col.t < tMin && col.t >= 0) {
                tMin = col.t;
                closestCol = col;
            }
        }
        return closestCol;
    }

    Collision checkVertices(glm::vec3 p, glm::vec3 d) {
        Collision closestCol = Collision{glm::vec3(), FLT_MAX, normal, normal};
        float tMin = FLT_MAX;

        for (int i = 0; i < 3; i++) {
            auto V = vertices[i];
            auto a = glm::dot(d, d);
            auto b = 2.0f * glm::dot(-d, (V - p));
            auto c = glm::dot(V - p, V - p) - 1;
            float t0 = -1.0f;
            float t1 = -1.0f;

            solveQuadratic(a, b, c, t0, t1);
            if (t0 > t1) std::swap(t0, t1);

            if (t0 < 0) {
                t0 = t1; // if t0 is negative, let's use t1 instead
                if (t0 < 0) continue; // both t0 and t1 are negative
            }

            float t = t0;
            auto P = p + d * t;

            Collision col = Collision{glm::vec3(), t, normal, P};
            if (col.t < tMin && col.t >= 0) {
                tMin = col.t;
                closestCol = col;
            }

        }

        return closestCol;
    }
};

#endif // TRIANGLE_H
