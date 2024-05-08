#include "collisionsystem.h"
#include "Engine/collisioncomponent.h"
#include "Engine/gameobject.h"
#include "Engine/uniformgrid.h"
#include "ellipsecollider.h"

CollisionSystem::CollisionSystem() :
    m_uniformGrid(UniformGrid(glm::vec3(-20, -20, -20), glm::vec3(20, 20, 20), 5))
{

}

CollisionSystem::CollisionSystem(UniformGrid& uniformGrid) : m_uniformGrid(uniformGrid){

}


void CollisionSystem::addGameObject(std::shared_ptr<GameObject> gameObject) {
    if (gameObject->getComponent<CollisionComponent>()->isStatic()) {
        m_staticObjs.push_back(gameObject);
    } else {
        m_dynamicObjs.push_back(gameObject);
    }
}

void CollisionSystem::removeGameObject(std::shared_ptr<GameObject> gameObject) {
    if (gameObject->getComponent<CollisionComponent>()->isStatic()) {
        m_staticObjs.erase(std::remove(m_staticObjs.begin(), m_staticObjs.end(), gameObject), m_staticObjs.end());
    } else {
        m_dynamicObjs.erase(std::remove(m_dynamicObjs.begin(), m_dynamicObjs.end(), gameObject), m_dynamicObjs.end());
    }
}


void CollisionSystem::update(float deltaTime) {
    // static-dynamic collisions
//    CYLINDER CODE BELOW
//    for (int i = 0; i < m_dynamicObjs.size(); i++) {
//        for (int j = 0; j < m_staticObjs.size(); j++) {
//            std::shared_ptr<GameObject>& gObj1 = m_staticObjs[i];
//            std::shared_ptr<GameObject>& gObj2 = m_dynamicObjs[j];
//            std::shared_ptr<CollisionComponent> cc1 = gObj1->getComponent<CollisionComponent>();
//            std::shared_ptr<CollisionComponent> cc2 = gObj2->getComponent<CollisionComponent>();
//            cc1->update(deltaTime);
//            cc2->update(deltaTime);

//            cc1->tryCollision(cc2);
//        }
//    }

    // Ellipsoid-triangle collisions
    for (int i = 0; i < m_dynamicObjs.size(); i++) {
        for (int j = 0; j < m_staticObjs.size(); j++) {
            std::shared_ptr<GameObject>& gObj1 = m_dynamicObjs[i];
            std::shared_ptr<GameObject>& gObj2 = m_staticObjs[j];
            std::shared_ptr<CollisionComponent> cc1 = gObj1->getComponent<CollisionComponent>();
            std::shared_ptr<CollisionComponent> cc2 = gObj2->getComponent<CollisionComponent>();
            cc1->update(deltaTime);
            cc2->update(deltaTime);
            if (cc1->getEllipsoidCollider()) {
                cc1->collideMesh(cc2);
            }
        }
    }

    // Update uniform grid
//    m_uniformGrid.update(m_dynamicObjs);
    // Reset rays
    for (int i = 0; i < m_dynamicObjs.size(); i++) {
        std::shared_ptr<GameObject>& gObj = m_dynamicObjs[i];
        std::shared_ptr<CollisionComponent> cc = gObj->getComponent<CollisionComponent>();
        if (cc->getRay()) {cc->getRay()->reset(); cc->update(0);}
    }

    // Ellipsoid-ellipsoid collisions
    for (int i = 0; i < m_dynamicObjs.size() - 1; i++) {
        for (int j = i + 1; j < m_dynamicObjs.size(); j++) {
            std::shared_ptr<GameObject>& gObj1 = m_dynamicObjs[j];
            std::shared_ptr<GameObject>& gObj2 = m_dynamicObjs[i];
            std::shared_ptr<CollisionComponent> cc1 = gObj1->getComponent<CollisionComponent>();
            std::shared_ptr<CollisionComponent> cc2 = gObj2->getComponent<CollisionComponent>();
            cc1->update(deltaTime);
            cc2->update(deltaTime);
//            if (cc1->getEllipsoidCollider()->canCollide(m_uniformGrid, cc2->getEllipsoidCollider())) {
            if (cc1->getRay()) { cc1->getRay()->intersect(cc2->getEllipsoidCollider());}
            else if (cc2->getRay()) { cc2->getRay()->intersect(cc1->getEllipsoidCollider());}
            else {
                cc1->collideEllipsoid(cc2);
            }
//                std::cout << "Uniform Grid Collision Test Accepted" << std::endl;
//            } else {
//                std::cout << "Uniform Grid Collision Test Ignored" << std::endl;
//            }
        }
    }

}
