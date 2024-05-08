#include "cameracomponent.h"
#include "Engine/gameobject.h"
#include "Engine/transformcomponent.h"
#include "Graphics/global.h"

CameraComponent::CameraComponent(std::weak_ptr<GameObject> parent, float sensitivity, bool isPlayerControlled, bool canScroll)
{
    m_parent = parent;
    m_camera = std::make_shared<Camera>();
    m_sens = sensitivity;
    m_isPlayerControlled = isPlayerControlled;
    m_canScroll = canScroll;
    if (parent.lock()->hasComponent<TransformComponent>()) {
        m_transform = parent.lock()->getComponent<TransformComponent>();
    }
    m_scroll = 10.0f;
    prevMousePos = glm::vec2(0.0f);
    m_angle = glm::vec2(0.0f);
}

void CameraComponent::update() {
    glm::vec2 drag = Global::input.getMouseDrag();
    if (m_isPlayerControlled) {
        glm::vec3 look = m_camera->getLook();
        m_camera->setPos(m_transform->getPos());
        glm::vec2 delta = drag;
        m_angle += delta * 0.002f;
        m_angle.y = glm::clamp(m_angle.y, -4.5f, 4.5f);
        float camX = glm::sin(m_angle.x);
        float camZ = glm::cos(m_angle.x);
        m_camera->setLook(-glm::vec3(-camX, m_angle.y, camZ));
        m_camera->setPos(m_transform->getPos() - look * m_scroll);
        if (m_canScroll) {
            float scrollDist = Global::input.getScrollDist();
            m_scroll += scrollDist;
            m_scroll = glm::clamp(m_scroll, 0.0f, 10.0f);
        }
    }
    Global::graphics.setCameraData(m_camera);
}

glm::vec3 CameraComponent::getLook() {
    return m_camera->getLook();
}

std::shared_ptr<Camera> CameraComponent::getCamera() {
    return m_camera;
}
