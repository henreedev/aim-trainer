#ifndef COLLIDER_H
#define COLLIDER_H

#include <iostream>
#include <memory>
#include <glm/vec3.hpp>

class TriMesh;
class Collision;
class CylinderCollider;
class EllipseCollider;

class Collider {
public:
    virtual bool isCollidingCyl(std::shared_ptr<CylinderCollider> other) = 0;
    virtual Collision collideCyl(std::shared_ptr<CylinderCollider> other) = 0;
    virtual void updatePos(glm::vec3 newPos) = 0;
};

#endif // COLLIDER_H
