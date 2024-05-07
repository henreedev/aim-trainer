#include "collisioncomponent.h"
#include "Engine/charactercontrollercomponent.h"
#include "Engine/collision.h"
#include "Engine/cylindercollider.h"
#include "Engine/gameobject.h"
#include "Engine/transformcomponent.h"
#include "Engine/ellipsecollider.h"

CollisionComponent::CollisionComponent(std::weak_ptr<GameObject> parent, std::shared_ptr<CylinderCollider> collider, bool isStatic) :
    m_collider(collider), m_isStatic(isStatic)
{
    m_parent = parent;
    if (parent.lock()->hasComponent<TransformComponent>()) {
        m_transform = parent.lock()->getComponent<TransformComponent>();
    }
}

CollisionComponent::CollisionComponent(std::weak_ptr<GameObject> parent, std::shared_ptr<EllipseCollider> collider, bool isStatic) :
    m_ellCollider(collider), m_isStatic(isStatic)
{
    m_parent = parent;
    if (parent.lock()->hasComponent<TransformComponent>()) {
        m_transform = parent.lock()->getComponent<TransformComponent>();
    }
}

CollisionComponent::CollisionComponent(std::weak_ptr<GameObject> parent, std::shared_ptr<TriMesh> mesh) {
    m_parent = parent;
    m_triMesh = mesh;
    m_isStatic = true;
}



void CollisionComponent::update(float deltaTime) {
    if (m_collider) {
        m_collider->updatePos(m_transform->getPos());
    }
    if (m_ellCollider) {
        m_ellCollider->updatePos(m_transform->getPos());
    }
}

void CollisionComponent::tryCollision(std::shared_ptr<CollisionComponent> other) {
    if (other->getCollider()->isCollidingCyl(m_collider)) {
        Collision col = other->getCollider()->collideCyl(m_collider);
        if (isStatic()) {
            // this static, other dynamic
            other->m_transform->translate(-col.mtv);
        } else if (other->isStatic()) {
            // other static, this dynamic
            m_transform->translate(col.mtv);
        } else {
            // both dynamic
            auto halfMtv = col.mtv / 2.0f;
            m_transform->translate(halfMtv);
            other->m_transform->translate(-halfMtv);
        }
        update(0);
        other->update(0);
    }
}

void CollisionComponent::collideMesh(std::shared_ptr<CollisionComponent> other) {
    if (m_ellCollider) {
        std::shared_ptr<TriMesh> mesh = other->getMesh();
        CollisionResult res = m_ellCollider->collideMesh(mesh);
        m_transform->setPos(res.finalPos);
        if (m_parent.lock()->hasComponent<CharacterControllerComponent>()) {
            m_parent.lock()->getComponent<CharacterControllerComponent>()->onCollision(res);
        }
        update(0);
        other->update(0);
    }
}


void CollisionComponent::collideEllipsoid(std::shared_ptr<CollisionComponent> other) {
    if (m_ellCollider) {
        std::shared_ptr<EllipseCollider> ellipsoid = other->getEllipsoidCollider();
        Collision col = m_ellCollider->collideEllipsoid(ellipsoid);
        auto halfMtv = col.mtv / 2.0f;
        m_transform->translate(halfMtv);
        other->m_transform->translate(-halfMtv);
        if (col.t > 0) {
            if (m_parent.lock()->hasComponent<CharacterControllerComponent>()) {
                m_parent.lock()->getComponent<CharacterControllerComponent>()->onCollision(col);
            }
            if (other->m_parent.lock()->hasComponent<CharacterControllerComponent>()) {
                other->m_parent.lock()->getComponent<CharacterControllerComponent>()->onCollision(col);
            }
        }
        update(0);
        other->update(0);
    }
}
std::shared_ptr<TriMesh> CollisionComponent::getMesh() {
    return m_triMesh;
}

bool CollisionComponent::isStatic() {return m_isStatic;}
void CollisionComponent::setStatic(bool isStatic) {m_isStatic = isStatic;}

std::shared_ptr<CylinderCollider> CollisionComponent::getCollider() {
    return m_collider;
}

std::shared_ptr<EllipseCollider> CollisionComponent::getEllipsoidCollider() {
    return m_ellCollider;
}
