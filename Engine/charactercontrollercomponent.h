#ifndef CHARACTERCONTROLLERCOMPONENT_H
#define CHARACTERCONTROLLERCOMPONENT_H


#include <iostream>
#include <memory>
#include "Engine/collisionresult.h"
#include "Engine/component.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

class CameraComponent;
class TransformComponent;


class CharacterControllerComponent : public Component
{
public:
    CharacterControllerComponent(std::weak_ptr<GameObject> parent, float moveSpeed, float jumpStrength);
    CharacterControllerComponent(std::weak_ptr<GameObject> parent);
    void update(float deltaTime, float gravity);
    void onCollision(CollisionResult c);
    void onCollision(Collision col);
    void setPath(std::vector<glm::vec3> path) {
        m_path = path;
        target = glm::vec2(m_path.back().x, m_path.back().z);
        m_path.pop_back();
        printPath(path);
    }
    void jump();
    std::vector<glm::vec3> m_path;
    // Helper function to print contents of a path (vector of vec3s)
    void printPath(const std::vector<glm::vec3>& path) {
//        std::cout << "Path: ";
//        for (const glm::vec3& point : path) {
//            std::cout << "(" << point.x << ", " << point.y << ", " << point.z << ") ";
//        }
//        std::cout << std::endl;
    }
    glm::vec2 target;
private:
    float m_moveSpeed = 3.0f;
    float m_jumpStrength = 4.0f;
    std::shared_ptr<CameraComponent> m_cc;
    std::shared_ptr<TransformComponent> m_tc;
    bool m_grounded;
    bool m_isCharacter;
    glm::vec3 m_vel;
    void updatePath();
};

#endif // CHARACTERCONTROLLERCOMPONENT_H
