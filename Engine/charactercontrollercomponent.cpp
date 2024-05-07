#include "Engine/collision.h"
#include "Engine/collisionresult.h"
#include "Engine/gameobject.h"
#include "GL/glew.h"
#include "charactercontrollercomponent.h"
#include "Engine/transformcomponent.h"
#include "GLFW/glfw3.h"
#include "Graphics/global.h"
#include "Engine/cameracomponent.h"

CharacterControllerComponent::CharacterControllerComponent(std::weak_ptr<GameObject> parent,
                                                           float moveSpeed, float jumpStrength) :
    m_moveSpeed(moveSpeed), m_jumpStrength(jumpStrength)
{
    m_isCharacter = true;
    m_parent = parent;
    if (parent.lock()->hasComponent<TransformComponent>()) {
        m_tc = parent.lock()->getComponent<TransformComponent>();
    }
    if (parent.lock()->hasComponent<CameraComponent>()) {
        m_cc = parent.lock()->getComponent<CameraComponent>();
    }
    m_vel = glm::vec3();
    target = glm::vec2();
}

CharacterControllerComponent::CharacterControllerComponent(std::weak_ptr<GameObject> parent)
{
    m_isCharacter = false;
    m_parent = parent;
    if (parent.lock()->hasComponent<TransformComponent>()) {
        m_tc = parent.lock()->getComponent<TransformComponent>();
    }
    m_vel = glm::vec3();
    target = glm::vec2();
    m_moveSpeed = 2.5f;
    m_jumpStrength = 2.0f;
}

void CharacterControllerComponent::jump() {
    std::cout << m_jumpStrength << std::endl;
    std::cout << m_jumpStrength << std::endl;
    std::cout << m_jumpStrength << std::endl;
    std::cout << m_jumpStrength << std::endl;
    m_vel.y = 4.0f;
}

void CharacterControllerComponent::onCollision(CollisionResult c){
    const float groundedThreshold = 0.5f;
    for (auto& col : c.collisions) {
        if (glm::dot(glm::vec3(0, 1, 0), col.normal) > groundedThreshold) {
            m_vel = glm::vec3();
            m_grounded = true;
        }
    }
}

void CharacterControllerComponent::onCollision(Collision col){
    const float groundedThreshold = 0.8f;
    if (glm::dot(glm::vec3(0, 1, 0), col.normal) > groundedThreshold) {
        m_vel = glm::vec3();
        m_grounded = true;
    }
}

void CharacterControllerComponent::update(float deltaTime, float gravity) {
    if (m_isCharacter) {
        glm::vec3 posAdjustment(0);
        glm::vec3 look = m_cc->getLook();

        if (Global::input.actionHeld(GLFW_KEY_W)) posAdjustment += glm::vec3(look.x, 0, look.z);
        if (Global::input.actionHeld(GLFW_KEY_S)) posAdjustment += glm::vec3(-look.x, 0, -look.z);
        if (Global::input.actionHeld(GLFW_KEY_A)) posAdjustment += glm::vec3(look.z, 0, -look.x);
        if (Global::input.actionHeld(GLFW_KEY_D)) posAdjustment += glm::vec3(-look.z, 0, look.x);

        posAdjustment = glm::normalize(posAdjustment) * m_moveSpeed * (float) deltaTime;
        if (glm::isnan(posAdjustment.x)) posAdjustment = glm::vec3(0.0);
        m_tc->translate(posAdjustment);
    } else {
        if (!glm::all(glm::equal(target, glm::vec2(0.0f)))) {
            glm::vec2 dir(glm::normalize(target - glm::vec2(m_tc->getPos().x, m_tc->getPos().z)));
            dir = dir * m_moveSpeed * (float) deltaTime;
            glm::vec3 posAdjustment(dir.x, 0, dir.y);
            if (glm::isnan(posAdjustment.x)) posAdjustment = glm::vec3(0.0);
            m_tc->translate(posAdjustment);
        }
        updatePath();
    }

    m_tc->translate(m_vel * (float) deltaTime);
    m_vel.y -= gravity * (float) deltaTime;

    if(m_tc->getPos().y <= 0.5f) {
        m_tc->setPos(glm::vec3(m_tc->getPos().x, 0.5f, m_tc->getPos().z));
        m_vel.y = 0.0f;
        m_grounded = true;
    }

    if (m_isCharacter) {
        if (m_grounded && Global::input.actionHeld(GLFW_KEY_SPACE)){
            m_vel.y = 5.0f;
        } else if (m_grounded) {
            m_vel.y = 0.0f;
        }
    }
    m_grounded = false;
}

void CharacterControllerComponent::updatePath() {
    const float minDist = 0.1f;
    if (!m_path.empty() && !glm::all(glm::equal(target, glm::vec2(0.0f)))) {
        glm::vec3 pos = m_tc->getPos();
        glm::vec2 pos2d(pos.x, pos.z);
//        std::cout << glm::distance(target, pos2d) << std::endl;
        if (glm::distance(target, pos2d) < minDist) {
            if (!m_path.empty()) {
//                std::cout << "popped path element" << std::endl;
                target = glm::vec2(m_path.back().x, m_path.back().z);
                m_path.pop_back();
            } else {
                target = glm::vec2(0.0f);
            }
        }
    }
}
