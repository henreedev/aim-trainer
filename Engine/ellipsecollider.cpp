#include "ellipsecollider.h"
#include "Engine/collision.h"
#include "Engine/collisionresult.h"
#include "Engine/meshloader.h"
#include "Engine/transformcomponent.h"
#include "Engine/uniformgrid.h"

EllipseCollider::EllipseCollider(std::shared_ptr<TransformComponent> transformPtr) // Assume it is a unit circle unless modified by the transform
{
    m_transform = transformPtr;
    m_pos = glm::vec3();
    m_lastPos = glm::vec3();
}

EllipseCollider::~EllipseCollider() {

}

bool areEqual(const std::vector<int>& v1, const std::vector<int>& v2) {
    // Check if sizes are equal
    if (v1.size() != v2.size()) {
        return false;
    }

    // Compare each element
    for (size_t i = 0; i < v1.size(); ++i) {
        if (v1[i] != v2[i]) {
            return false;
        }
    }

    // If sizes are equal and all elements are equal, vectors are equal
    return true;
}


Collision EllipseCollider::collideCyl(std::shared_ptr<CylinderCollider> other) {
    return Collision();
}

bool EllipseCollider::isCollidingCyl(std::shared_ptr<CylinderCollider> other) {
    return false;
}

void EllipseCollider::setUniformGridIndices(std::vector<std::vector<int>> indices) {
    m_gridIndices = indices;
}

bool EllipseCollider::canCollide(UniformGrid uniformGrid, std::shared_ptr<EllipseCollider> other) {
    for (auto& index : m_gridIndices) {
        for(auto& otherIndex : other->m_gridIndices) {
            if(areEqual(index, otherIndex)) {
                return true;
            }
        }
    }
    return false;
}

CollisionResult EllipseCollider::collideMesh(std::shared_ptr<TriMesh> mesh) {
    std::vector<Collision> collisions;
    auto curr_pos = m_lastPos;
    auto next_pos = m_pos;
    // special case on first frame
    if (m_lastPos == glm::vec3(0.0f)){
        return CollisionResult(m_pos, collisions);
    }
    for (int i = 0; i < MAX_TRANSLATIONS; i++) {
        Collision c = getClosestCollision(curr_pos, next_pos, mesh);
        if (c.t > 1.0f || c.t < 0.0f) {
            return CollisionResult(next_pos, collisions);
        } else {
            curr_pos = doNudge(curr_pos, c, mesh);
            auto d = next_pos - curr_pos;
            auto d_corrected = d - glm::dot(d, c.normal) * c.normal;
            next_pos = curr_pos + d_corrected;
            collisions.push_back(c);
        }
    }

    return CollisionResult(curr_pos, collisions);
}

Collision EllipseCollider::collideEllipsoid(std::shared_ptr<EllipseCollider> other) {
    glm::vec3 other_scale = other->m_transform->getScale();
    float other_avg_scale = (other_scale.x + other_scale.y + other_scale.z) / 3.0;
    glm::vec3 scale = m_transform->getScale();
    float avg_scale = (scale.x + scale.y + scale.z) / 3.0;
    float other_r = 0.5 * other_avg_scale;
    float r = 0.5 * avg_scale;
    auto diff = other->m_transform->getPos() - m_transform->getPos();
    float dist = glm::length(diff);
    if (dist < r + other_r) {
        // Collision
        float mtvDist = dist - r - other_r;
        glm::vec3 normal = glm::normalize(diff);
        glm::vec3 mtv = normal * mtvDist;
        return Collision(mtv, 1, normal, glm::vec3(0));
    } else {
        return Collision(glm::vec3(0), 0.0f, glm::vec3(0), glm::vec3(0));
    }
}

glm::vec3 EllipseCollider::doNudge(glm::vec3 curr_pos, Collision c, std::shared_ptr<TriMesh> mesh){
    const float nudgeStr = 0.001f;
    auto nudge = c.normal;
    auto pos_nudged = c.pointOfContact + nudge * nudgeStr;
    for (int i = 0; i < MAX_NUDGES; i++) {
        auto nudge_collision = getClosestCollision(curr_pos, pos_nudged, mesh);
        if (nudge_collision.t < 0 || nudge_collision.t > 1) {
            curr_pos = pos_nudged;
            break;
        } else {
            if (glm::length(nudge_collision.normal - nudge) < 0.0001f || glm::length(nudge_collision.normal + nudge) < 0.0001f) {
                nudge = -nudge_collision.normal;
            } else {
                nudge = nudge_collision.normal;
            }
            pos_nudged = nudge_collision.pointOfContact + nudge * nudgeStr;
        }
    }
    return pos_nudged;
}

Collision EllipseCollider::getClosestCollision(glm::vec3 initialPos, glm::vec3 nextPos, std::shared_ptr<TriMesh> mesh) {
    // create movement ray
    glm::vec4 p = glm::vec4(initialPos,1);
    glm::vec4 d = glm::vec4(nextPos - initialPos, 0);

    // TODO transform to unit sphere space
    float tMin = FLT_MAX;
    Collision closestCol{glm::vec3(), tMin, glm::vec3(), glm::vec3()};

    // Find triangles that should be collision-tested
    glm::vec4 aabbMinHomo = glm::vec4(-0.5, -0.5, -0.5, 1);
    glm::vec4 aabbMaxHomo = glm::vec4(0.5, 0.5, 0.5, 1);
    glm::vec3 aabbMin = glm::vec3(m_transform->getModelMatrix() * aabbMinHomo);
    glm::vec3 aabbMax = glm::vec3(m_transform->getModelMatrix() * aabbMaxHomo);

    std::vector<Triangle> tris = MeshLoader::traverseBVH(mesh->bvhRoot, aabbMin, aabbMax);
    for (auto& tri : tris) {
        // CHECK FACE COLLISION
        Collision c = tri.checkRaycast(p, d, true);
        if (c.t < tMin && c.t >= 0) {
            tMin = c.t;
            closestCol = c;
            continue; // face collision means edge and vertex can be ignored
        }
        // CHECK EDGE COLLISION
        c = tri.checkEdges(p, d);
        if (c.t < tMin && c.t >= 0) {
            tMin = c.t;
            closestCol = c;
        }
        // CHECK VERTEX COLLISION
        c = tri.checkVertices(p, d);
        if (c.t < tMin && c.t >= 0) {
            tMin = c.t;
            closestCol = c;
        }

    }
    return closestCol;
}
