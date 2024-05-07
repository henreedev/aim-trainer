#include "charactercontrollersystem.h"
#include "Engine/cameracomponent.h"
#include "Engine/charactercontrollercomponent.h"
#include "Engine/gameobject.h"

CharacterControllerSystem::CharacterControllerSystem(float gravity)
{
    m_gravity = gravity;
}


void CharacterControllerSystem::update(float deltaTime) {
    for (const std::shared_ptr<GameObject>& gObj : m_objects) {
        std::shared_ptr<CharacterControllerComponent> ccc = gObj->getComponent<CharacterControllerComponent>();
        ccc->update(deltaTime, m_gravity);
    };
}
