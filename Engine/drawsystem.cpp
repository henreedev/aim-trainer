#include "Engine/drawsystem.h"
#include "Engine/drawcomponent.h"
#include "Engine/gameobject.h"

DrawSystem::DrawSystem() {}


void DrawSystem::draw() {
    for (const std::shared_ptr<GameObject>& gObj : m_objects) {
        std::shared_ptr<DrawComponent> drawComponent = gObj->getComponent<DrawComponent>();
        drawComponent->draw();
    };
}
