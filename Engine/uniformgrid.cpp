#include "uniformgrid.h"
#include "Engine/Collider.h"
#include "Engine/collisioncomponent.h"
#include "Engine/gameobject.h"
#include "Engine/transformcomponent.h"
#include "ellipsecollider.h"

UniformGrid::UniformGrid(glm::vec3 dimsMin, glm::vec3 dimsMax, int subdivisions) :
    dimsMin(dimsMin), dimsMax(dimsMax), size(subdivisions)
{
    dimsDiff = dimsMax - dimsMin;
    // Resize the 3-dimensional vector
    grid.resize(subdivisions);
    for (int i = 0; i < subdivisions; ++i) {
        grid[i].resize(subdivisions);
        for (int j = 0; j < subdivisions; ++j) {
            grid[i][j].resize(subdivisions);
        }
    }
}

bool areEqual(const glm::vec3& v1, const glm::vec3& v2) {
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

void UniformGrid::update(std::vector<std::shared_ptr<GameObject>> gObjs) {
    for (auto& gObj : gObjs) {
        if (!gObj->getComponent<CollisionComponent>()->getEllipsoidCollider()) {return;}
        std::shared_ptr<TransformComponent> transform = gObj->getComponent<TransformComponent>();
        glm::vec4 aabbMinHomo = glm::vec4(-0.5, -0.5, -0.5, 1);
        glm::vec4 aabbMaxHomo = glm::vec4(0.5, 0.5, 0.5, 1);
        glm::vec3 aabbMin = glm::vec3(transform->getModelMatrix() * aabbMinHomo);
        glm::vec3 aabbMax = glm::vec3(transform->getModelMatrix() * aabbMaxHomo);
        glm::vec3 ratioMin = (aabbMin - dimsMin) / dimsDiff;
        glm::vec3 ratioMax = (aabbMax - dimsMin) / dimsDiff;
        glm::vec3 indicesMin = glm::floor(ratioMin * (float)size);
        glm::vec3 indicesMax = glm::floor(ratioMax * (float)size);
        if(areEqual(indicesMin, indicesMax)) {
            std::shared_ptr<EllipseCollider> collider = gObj->getComponent<CollisionComponent>()->getEllipsoidCollider();
            std::vector<std::vector<int>> indices = {{(int)indicesMin.x, (int)indicesMin.y, (int)indicesMin.z}};
            collider->setUniformGridIndices(indices);
        } else {
            std::shared_ptr<EllipseCollider> collider = gObj->getComponent<CollisionComponent>()->getEllipsoidCollider();
            std::vector<std::vector<int>> indices;
            for (int i = indicesMin[0]; i <= indicesMax[0]; i++) {
                for (int j = indicesMin[1]; j <= indicesMax[1]; j++) {
                    for (int k = indicesMin[2]; k <= indicesMin[2]; k++) {
                        indices.push_back({i, j, k});
                    }
                }
            }
            collider->setUniformGridIndices(indices);
        }
    }

}
