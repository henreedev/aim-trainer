#include "cylindercollider.h"
#include "Engine/collision.h"
#include "glm/geometric.hpp"
#include "glm/vec2.hpp"
#include <cmath>

CylinderCollider::CylinderCollider(float radius, float height, std::shared_ptr<TransformComponent> transformPtr) :
    m_radius(radius), m_height(height), m_transform(transformPtr)
{
    m_pos = glm::vec3();
}

bool CylinderCollider::isCollidingCyl(std::shared_ptr<CylinderCollider> other) {
    // Check circle collision:
    float x1 = m_pos.x;
    float z1 = m_pos.z;
    float x2 = other->m_pos.x;
    float z2 = other->m_pos.z;
    float r1 = m_radius;
    float r2 = other->m_radius;
    bool hozColliding = sqrt((x1 - x2) * (x1 - x2) + (z1 - z2) * (z1 - z2)) < (r1 + r2);
    // Check Vertical collision:
    float y = m_pos.y;
    float y2 = other->m_pos.y;
    float h = m_height;
    float h2 = other->m_height;
    bool vertColliding = (y < y2 + h2) && (y2 < y + h);
    return hozColliding && vertColliding;
}

Collision CylinderCollider::collideCyl(std::shared_ptr<CylinderCollider> other) {
    Collision col;
    col.mtv = glm::vec3();
    // Check circle collision:
    float x1 = m_pos.x;
    float z1 = m_pos.z;
    float x2 = other->m_pos.x;
    float z2 = other->m_pos.z;

    float r1 = m_radius;
    float r2 = other->m_radius;
    float mtvLen = 9999990.0;
    if (sqrt((x1 - x2) * (x1 - x2) + (z1 - z2) * (z1 - z2)) < (r1 + r2)) {
        glm::vec2 direction = glm::vec2(x2, z2) - glm::vec2(x1, z1);
        float len = glm::length(direction);
        glm::vec2 mtv = direction / len * ((r1 + r2) - len);
        col.mtv = glm::vec3(mtv.x, 0, mtv.y);
        mtvLen = ((r1 + r2) - len);
    }
    // Check Vertical collision:
    float y = m_pos.y;
    float y2 = other->m_pos.y;
    float h = m_height;
    float h2 = other->m_height;

    if ( y < y2 + h2 && y2 < y + h) {
        float aRight = (y2 + h2) - y;
        float aLeft  = (y + h) - y2;
        if (aRight < aLeft) {
            if (abs(aRight) < mtvLen) {
                mtvLen = abs(aRight);
                col.mtv = glm::vec3(0, -aRight, 0);
            }
        }
        else {
            if (abs(aLeft) < mtvLen) {
                mtvLen = abs(aLeft);
                col.mtv = glm::vec3(0, aLeft, 0);
            }
        }
    }

    return col;
}
