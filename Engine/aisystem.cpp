#include "aisystem.h"
#include "Engine/aicomponent.h"
#include "Engine/charactercontrollercomponent.h"
#include "Engine/gameobject.h"
#include "Engine/transformcomponent.h"

NavMesh* AiSystem::navMesh = nullptr;
std::shared_ptr<GameObject> AiSystem::player = nullptr;

AiSystem::AiSystem()
{
    m_timer = 0.0f;
}

void AiSystem::update(float deltaTime) {
    if (m_timer >= 3.0f) {
        m_timer = 0.0f;
        std::cout << "updating" << std::endl;
        if (player != nullptr) {
            for (const std::shared_ptr<GameObject>& gObj : m_objects) {
                std::shared_ptr<AiComponent> aiComponent= gObj->getComponent<AiComponent>();
                std::shared_ptr<CharacterControllerComponent> ccc = gObj->getComponent<CharacterControllerComponent>();
                glm::vec3 start(gObj->getComponent<TransformComponent>()->getPos());
                glm::vec3 fin(player->getComponent<TransformComponent>()->getPos());
                ccc->setPath(navMesh->findPath(start, fin));
                aiComponent->update(deltaTime);
            };
        }
    }
    m_timer += deltaTime;
}
