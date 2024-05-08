#ifndef COLLISIONRESULT_H
#define COLLISIONRESULT_H

#include "glm/ext/vector_float3.hpp"
#include <vector>
#include "collision.h"

struct CollisionResult {
    glm::vec3 finalPos;
    std::vector<Collision> collisions;
    CollisionResult(const glm::vec3& _finalPos, const std::vector<Collision>& _collisions)
        : finalPos(_finalPos), collisions(_collisions) {}
    CollisionResult(){}
};

#endif // COLLISIONRESULT_H
