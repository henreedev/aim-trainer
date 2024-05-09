#ifndef SCENARIO_H
#define SCENARIO_H

#include "Engine/collisioncomponent.h"
#include "Engine/drawcomponent.h"
#include "Engine/ellipsecollider.h"
#include "Engine/gameobject.h"
#include "Engine/healthcomponent.h"
#include "Engine/scenariocomponent.h"
#include "Engine/transformcomponent.h"
#include "glm/vec3.hpp"


class DummyMaker {
public:
    static std::shared_ptr<GameObject> makeDummy(glm::vec3 position, float health, float scale);
};

class ScenarioMaker {
public:
    static std::shared_ptr<GameObject> makeScenario(Scenario type, float highScore) {
        std::shared_ptr<GameObject> scen = std::make_shared<GameObject>();
        std::weak_ptr<GameObject> scen_weak = scen;
        std::shared_ptr<ScenarioComponent> sc = std::make_shared<ScenarioComponent>(scen_weak, type, highScore);
        scen->addComponent<ScenarioComponent>(sc);
        return scen;
    }
};

#endif // SCENARIO_H
