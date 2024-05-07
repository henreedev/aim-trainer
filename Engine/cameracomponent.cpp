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
}

void CameraComponent::update() {
    if (m_isPlayerControlled) {
        glm::vec2 drag = Global::input.getMouseDrag();
        glm::vec3 look = m_camera->getLook();
        glm::vec3 vertRotAxis = look.x == 0.0f && look.z == 0.0f ? glm::vec3(1, 0, 0) : glm::normalize(glm::vec3(look.z, 0, look.x));
        m_camera->setPos(m_transform->getPos());
        m_camera->rotate(drag.y * m_sens, vertRotAxis);
        m_camera->rotate(drag.x * m_sens * -1, glm::vec3(0, 1, 0));
        look = m_camera->getLook();
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
