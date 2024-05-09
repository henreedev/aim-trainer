#ifndef SCENARIOCOMPONENT_H
#define SCENARIOCOMPONENT_H

#include "Engine/component.h"
#include "Engine/scenarioEnum.h"
#include "glm/ext/vector_float3.hpp"
#include <iostream>
#include <random>


class ScenarioComponent : public Component {
public:
    float scenarioTimer = 60.0f;
    float score = 0.0f;
    float highScore = 0.0f;
    const glm::vec3 defaultSpawn = glm::vec3(0, 5, -10);
    const float gridDist = 1.0f;
    Scenario scenario;
    ScenarioComponent(std::weak_ptr<GameObject> parent_weak, Scenario scenario, float highScore)
        : scenario(scenario), highScore(highScore)
    {
        m_parent = parent_weak;
    }

    void begin();
    void end();

    void update(float deltaTime);

};

#endif // SCENARIOCOMPONENT_H
