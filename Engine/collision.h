#ifndef COLLISION_H
#define COLLISION_H

#include <memory>
#include <glm/vec3.hpp>

class GameObject;

struct Collision {
    glm::vec3 mtv;
    float t;
    glm::vec3 normal;
    glm::vec3 pointOfContact;
//    std::shared_ptr<GameObject> other;
};

#endif // COLLISION_H
