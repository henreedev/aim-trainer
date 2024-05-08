#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "Engine/raycast.h"
#include "component.h"
#include "cylindercollider.h"
class Collider;

class TransformComponent;
class TriMesh;

class CollisionComponent : public Component
{
public:
    CollisionComponent(std::weak_ptr<GameObject> parent, std::shared_ptr<CylinderCollider> collider, bool isStatic);
    CollisionComponent(std::weak_ptr<GameObject> parent, std::shared_ptr<EllipseCollider> collider, bool isStatic);
    CollisionComponent(std::weak_ptr<GameObject> parent, std::shared_ptr<Ray> collider);
    CollisionComponent(std::weak_ptr<GameObject> parent, std::shared_ptr<TriMesh> mesh);

    void update(float deltaTime);
    void tryCollision(std::shared_ptr<CollisionComponent> other);
    void collideMesh(std::shared_ptr<CollisionComponent> other);
    std::shared_ptr<TriMesh> getMesh();
    std::shared_ptr<CylinderCollider> getCollider();
    std::shared_ptr<Ray> getRay();
    bool isStatic();
    void setStatic(bool isStatic);
    std::shared_ptr<EllipseCollider> getEllipsoidCollider();
    void collideEllipsoid(std::shared_ptr<CollisionComponent> other);
private:
    std::shared_ptr<CylinderCollider> m_collider;
    std::shared_ptr<EllipseCollider> m_ellCollider;
    std::shared_ptr<Ray> m_ray;
    std::shared_ptr<TriMesh> m_triMesh;
    std::shared_ptr<TransformComponent> m_transform;
    bool m_isStatic;
};

#endif // COLLISIONCOMPONENT_H
