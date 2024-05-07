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
    Collision(const glm::vec3& _mtv, float _t, const glm::vec3& _normal, const glm::vec3& _pointOfContact, const std::shared_ptr<GameObject>& _other)
        : mtv(_mtv), t(_t), normal(_normal), pointOfContact(_pointOfContact) {}
};

#endif // COLLISION_H
