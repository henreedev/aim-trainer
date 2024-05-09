#ifndef UPDATESYSTEM_H
#define UPDATESYSTEM_H

#include "Engine/System.h"
#include "Engine/dummycomponent.h"
#include "Engine/gameobject.h"
#include "Engine/scenariocomponent.h"

class UpdateSystem : public System {
public:
    UpdateSystem(){}
    ~UpdateSystem(){}
    void update(float deltaTime) {
        for (const std::shared_ptr<GameObject>& gObj : m_objects) {
            if (gObj->hasComponent<ScenarioComponent>())  {

                std::shared_ptr<ScenarioComponent> scenarioComponent = gObj->getComponent<ScenarioComponent>();
                scenarioComponent->update(deltaTime);
            }
            if (gObj->hasComponent<DummyComponent>())  {

                std::shared_ptr<DummyComponent> dummyComponent = gObj->getComponent<DummyComponent>();
                dummyComponent->update(deltaTime);
            }
        };
    }
};
#endif // UPDATESYSTEM_H
