#ifndef DUMMYCOMPONENT_H
#define DUMMYCOMPONENT_H

#include "Engine/collisioncomponent.h"
#include "Engine/randomgen.h"
#include "Engine/scenarioEnum.h"
#include "component.h"

class DummyComponent : public Component
{
public:
    Scenario scenario;
    bool hasGravity;
    const float minStrafe = 0.4f;
    const float maxStrafe = 1.5f;
    float strafeTimer = 1.0f;
    float gravity;
    float y_vel = 0.0f;
    bool isGoingLeft = false;
    const float speed = 1.0;

    DummyComponent(std::weak_ptr<GameObject> parent_weak, Scenario scenario, bool hasGravity, float gravity)
        : scenario(scenario), hasGravity(hasGravity), gravity(gravity) {
        m_parent = parent_weak;
    }

    float pickDuration() {
        auto rand = Random::generateRandomFloat(minStrafe, maxStrafe);
        strafeTimer = rand;
        isGoingLeft = !isGoingLeft;
        return rand;
    }

    void bounceOnCollision() {
        y_vel = 3.0f;
    }

    void update(float deltaTime) {
        strafeTimer -= deltaTime;
        if (strafeTimer < 0) {
            pickDuration();
        }
        std::shared_ptr<CollisionComponent> cc = m_parent.lock()->getComponent<CollisionComponent>();
        std::shared_ptr<TransformComponent> tc = m_parent.lock()->getComponent<TransformComponent>();
        auto xSpeed = (isGoingLeft ? -speed : speed ) * deltaTime;
        tc->translate(glm::vec3(xSpeed, 0, 0));
        if (hasGravity) {
            y_vel -= gravity * deltaTime;
            tc->translate(glm::vec3(0, y_vel * deltaTime, 0));
        }


    }

};

#endif // DUMMYCOMPONENT_H
