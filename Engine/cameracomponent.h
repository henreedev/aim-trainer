#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "GL/glew.h"
#include "Engine/component.h"
#include "Graphics/camera.h"
#include <memory>

class TransformComponent;

class CameraComponent : public Component
{
public:
    CameraComponent(std::weak_ptr<GameObject> parent, float sensitivity = 0.002, bool isPlayerControlled = true, bool canScroll = false);
    std::shared_ptr<Camera> getCamera();
    glm::vec3 getLook();
    void update();
private:
    std::shared_ptr<Camera> m_camera;
    bool m_isPlayerControlled;
    bool m_canScroll;
    float m_sens;
    float m_scroll;
    std::shared_ptr<TransformComponent> m_transform;
};

#endif // CAMERACOMPONENT_H
