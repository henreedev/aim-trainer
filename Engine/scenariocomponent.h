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
    glm::vec3 lastPos = glm::vec3(0);
    static float staticScore;
    float highScore = 0.0f;
    const glm::vec3 defaultSpawn = glm::vec3(0, 2.5, -7.5);
    const float gridDist = 1.5f;
    Scenario scenario;
    void makeGrid();
    void makeReact();
    ScenarioComponent(std::weak_ptr<GameObject> parent_weak, Scenario scenario, float highScore)
        : scenario(scenario), highScore(highScore)
    {
        m_parent = parent_weak;
    }

    void begin();
    void end();
    bool finished = false;
    bool everyOther = true;
    void update(float deltaTime);

};

#endif // SCENARIOCOMPONENT_H
