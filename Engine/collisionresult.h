#ifndef COLLISIONRESULT_H
#define COLLISIONRESULT_H

#include "glm/ext/vector_float3.hpp"
#include <vector>
class Collision;

struct CollisionResult {
    glm::vec3 finalPos;
    std::vector<Collision> collisions;
};

#endif // COLLISIONRESULT_H
