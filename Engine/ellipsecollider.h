#ifndef ELLIPSECOLLIDER_H
#define ELLIPSECOLLIDER_H

#include "Collider.h"
#include "Engine/collisionresult.h"
#include <cmath>

class UniformGrid;

class TransformComponent;

class EllipseCollider : public Collider
{
public:
    EllipseCollider(std::shared_ptr<TransformComponent> transformPtr);
    ~EllipseCollider();
    bool isCollidingCyl(std::shared_ptr<CylinderCollider> other) override;
    Collision collideCyl(std::shared_ptr<CylinderCollider> other) override;
    CollisionResult collideMesh(std::shared_ptr<TriMesh> mesh);
    glm::vec3 doNudge(glm::vec3 curr_pos, Collision c, std::shared_ptr<TriMesh> mesh);
    void updatePos(glm::vec3 newPos) override {
        m_lastPos = m_pos;
        m_pos = newPos;
    };
    void setUniformGridIndices(std::vector<std::vector<int>> indices);
    bool canCollide(UniformGrid uniformGrid, std::shared_ptr<EllipseCollider> other);
    Collision getClosestCollision(glm::vec3 initialPos, glm::vec3 nextPos, std::shared_ptr<TriMesh> mesh);
    const int MAX_TRANSLATIONS = 3;
    const int MAX_NUDGES = 3;
    static bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
    {
        float discr = b * b - 4 * a * c;
        if (discr < 0) return false;
        else if (discr == 0) x0 = x1 = - 0.5 * b / a;
        else {
            float q = (b > 0) ?
                          -0.5 * (b + sqrt(discr)) :
                          -0.5 * (b - sqrt(discr));
            x0 = q / a;
            x1 = c / q;
        }
        if (x0 > x1) std::swap(x0, x1);

        return true;
    }
    Collision collideEllipsoid(std::shared_ptr<EllipseCollider>);
protected:

    std::shared_ptr<TransformComponent> m_transform;
    glm::vec3 m_lastPos;
    glm::vec3 m_pos;
    std::vector<std::vector<int>> m_gridIndices;
};


#endif // ELLIPSECOLLIDER_H
