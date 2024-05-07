#ifndef CYLINDERCOLLIDER_H
#define CYLINDERCOLLIDER_H

#include "Engine/Collider.h"
#include "Engine/collision.h"

class TransformComponent;

class CylinderCollider : public Collider
{
public:
    CylinderCollider(float radius, float height, std::shared_ptr<TransformComponent> transformPtr);
    bool isCollidingCyl(std::shared_ptr<CylinderCollider> other) override;
    Collision collideCyl(std::shared_ptr<CylinderCollider> other) override;
    void updatePos(glm::vec3 newPos) override {
        m_pos = newPos;
    };
private:
    glm::vec3 m_pos;
    float m_radius;
    float m_height;
    std::shared_ptr<TransformComponent> m_transform;
};

#endif // CYLINDERCOLLIDER_H
