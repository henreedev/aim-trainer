#include "scenario.h"
#include "Engine/gameworld.h"
#include "Warmup/gamescreen.h"

std::shared_ptr<GameObject> DummyMaker::makeDummy(glm::vec3 position, float health, float scale){
    // MAKE dummy GAMEOBJECT
    std::shared_ptr<GameObject> dummy = std::make_shared<GameObject>();
    std::weak_ptr<GameObject> dummy_weak = dummy;
    std::shared_ptr<TransformComponent> tc = std::make_shared<TransformComponent>(dummy_weak, position, scale);
    dummy->addComponent<TransformComponent>(tc);
    dummy->addComponent<DrawComponent>(std::make_shared<DrawComponent>(dummy_weak, "sphere", "grass"));

    std::shared_ptr<EllipseCollider> collider = std::make_shared<EllipseCollider>(tc);

    std::shared_ptr<CollisionComponent> coc = std::make_shared<CollisionComponent>(dummy_weak, collider, false);
    dummy->addComponent<CollisionComponent>(coc);

    std::shared_ptr<HealthComponent> hc = std::make_shared<HealthComponent>(dummy_weak, health, health);
    dummy->addComponent<HealthComponent>(hc);
    GameWorld::gameWorldInstance->addGameObject(dummy);
    GameScreen::dummiesAlive++;

    return dummy;
}
