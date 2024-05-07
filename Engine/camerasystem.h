#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H


#include "Engine/System.h"
#include "Engine/gameobject.h"
#include "Engine/cameracomponent.h"
class CameraSystem : public System
{
public:
    CameraSystem(){}
    void update(float deltaTime) {
        for (const std::shared_ptr<GameObject>& gObj : m_objects) {
            std::shared_ptr<CameraComponent> cameraComponent = gObj->getComponent<CameraComponent>();
            cameraComponent->update();
        };
    }
};

#endif // CAMERASYSTEM_H
