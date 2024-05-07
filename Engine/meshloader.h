#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "Engine/navmesh.h"
#include "Graphics/modeltransform.h"
#include "glm/geometric.hpp"
#include "glm/vec3.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include "Engine/collision.h"

struct BVHNode {
    glm::vec3 aabbMin = glm::vec3(1e30f);
    glm::vec3 aabbMax = glm::vec3(-1e30f);
    BVHNode* leftChild;
    BVHNode* rightChild;
    bool isLeaf = true;
    Triangle triangle;
};

struct TriMesh {
    std::vector<Triangle> triangles;
    std::shared_ptr<ModelTransform> transform;
    BVHNode* bvhRoot;
    NavMesh* navmesh;
};

class MeshLoader {
public:
    static std::shared_ptr<TriMesh> getMeshFromVertices(std::vector<glm::vec3> vertices, std::shared_ptr<ModelTransform> transform) {
        std::shared_ptr<TriMesh> trimesh = std::make_shared<TriMesh>();
        // Add a triangle to the trimesh each loop
        for (int i = 0; i < vertices.size() / 3; i++) {
            auto modelMat = transform->getModelMatrix();
            auto firstIndex = i * 3;

            auto secondIndex = firstIndex + 1;
            auto thirdIndex = firstIndex + 2;
            auto A_homo = modelMat * glm::vec4(vertices[firstIndex], 1.0f);
            auto A = glm::vec3(A_homo);
            auto B_homo = modelMat * glm::vec4(vertices[secondIndex], 1.0f);
            auto B = glm::vec3(B_homo);
            auto C_homo = modelMat * glm::vec4(vertices[thirdIndex], 1.0f);
            auto C = glm::vec3(C_homo);
            auto normal = glm::normalize(glm::cross(B-A, C-A));
            std::vector<glm::vec3> vertices = {A, B, C};
            Triangle tri = {vertices, normal, (A + B + C) / 3.0f};
            trimesh->triangles.push_back(tri);
        }
        trimesh->bvhRoot = new BVHNode();
        trimesh->bvhRoot->leftChild = new BVHNode();
        trimesh->bvhRoot->rightChild = new BVHNode();
        createBVH(trimesh->triangles, trimesh->bvhRoot);
        return trimesh;
    }

    static void addNavmeshToTrimesh(std::vector<glm::vec3> vertices, std::shared_ptr<ModelTransform> transform, std::shared_ptr<TriMesh> trimesh) {
        NavMesh* navmesh = new NavMesh();
        // Add a triangle to the navmesh each loop
        for (int i = 0; i < vertices.size() / 3; i++) {
            auto modelMat = transform->getModelMatrix();
            auto firstIndex = i * 3;

            auto secondIndex = firstIndex + 1;
            auto thirdIndex = firstIndex + 2;
            auto A_homo = modelMat * glm::vec4(vertices[firstIndex], 1.0f);
            auto A = glm::vec3(A_homo);
            auto B_homo = modelMat * glm::vec4(vertices[secondIndex], 1.0f);
            auto B = glm::vec3(B_homo);
            auto C_homo = modelMat * glm::vec4(vertices[thirdIndex], 1.0f);
            auto C = glm::vec3(C_homo);
            auto normal = glm::normalize(glm::cross(B-A, C-A));
            std::vector<glm::vec3> vertices = {A, B, C};
            Triangle tri = {vertices, normal, (A + B + C) / 3.0f};
            navmesh->triangles.push_back(tri);
        }
        navmesh->generateNeighbors();
        trimesh->navmesh = navmesh;
    }

    static std::vector<Triangle> traverseBVH(BVHNode* node, glm::vec3 aabbMin, glm::vec3 aabbMax) {
        std::vector<Triangle> tris;
        if (intersectAABB(node->aabbMin, node->aabbMax, aabbMin, aabbMax)) {
            if (node->isLeaf) {
                tris.push_back(node->triangle);
            } else {
                auto it = tris.begin();
                auto leftRes = traverseBVH(node->leftChild, aabbMin, aabbMax);
                auto rightRes = traverseBVH(node->rightChild, aabbMin, aabbMax);
                if (leftRes.size() > 0) {
                    tris.insert(it, leftRes.begin(), leftRes.end());
                }
                if (rightRes.size() > 0) {
                    it = tris.begin();
                    tris.insert(it, rightRes.begin(), rightRes.end());
                }
            }
        }
        return tris;
    }

    static bool intersectAABB(const glm::vec3& minA, const glm::vec3& maxA, const glm::vec3& minB, const glm::vec3& maxB) {
        if (maxA.x < minB.x || minA.x > maxB.x) return false;
        if (maxA.y < minB.y || minA.y > maxB.y) return false;
        if (maxA.z < minB.z || minA.z > maxB.z) return false;

        return true;
    }


    static void createBVH(std::vector<Triangle> tris, BVHNode* node) {
        for (auto& tri : tris) {
            setAABB(tri, node);
        }
        if (tris.size() > 1) {
            node->isLeaf = false;
            glm::vec3 extent = node->aabbMax - node->aabbMin;
            int axis = 0;
            if (extent.y > extent.x) axis = 1;
            if (extent.z > extent[axis]) axis = 2;
            float splitPos = node->aabbMin[axis] + extent[axis] * 0.5f;
            std::vector<Triangle> leftTris;
            std::vector<Triangle> rightTris;
            for(auto& tri : tris) {
                if (tri.centroid[axis] < splitPos) {
                    leftTris.push_back(tri);
                } else {
                    rightTris.push_back(tri);
                }
            }
            if (leftTris.size() > 1 && rightTris.size() == 0) {
                auto half = leftTris.size() / 2;
                for (int i = 0; i < half; i++) {
                    rightTris.push_back(leftTris[leftTris.size() - 1]);
                    leftTris.pop_back();
                }
            } else if (rightTris.size() > 1 && leftTris.size() == 0) {
                auto half = rightTris.size() / 2;
                for (int i = 0; i < half; i++) {
                    leftTris.push_back(rightTris[rightTris.size() - 1]);
                    rightTris.pop_back();
                }
            }
            if (leftTris.size() > 0) {
                node->leftChild = new BVHNode();
                createBVH(leftTris, node->leftChild);
            }
            if (rightTris.size() > 0) {
                node->rightChild = new BVHNode();
                createBVH(rightTris, node->rightChild);
            }
        } else {
            node->isLeaf = true;
            node->triangle = tris[0];
        }

    }

    static void setAABB(Triangle tri, BVHNode* node) {
        node->aabbMin = vec3_fminf( node->aabbMin, tri.vertices[0] );
        node->aabbMin = vec3_fminf( node->aabbMin, tri.vertices[1] );
        node->aabbMin = vec3_fminf( node->aabbMin, tri.vertices[2] );
        node->aabbMax = vec3_fmaxf( node->aabbMax, tri.vertices[0] );
        node->aabbMax = vec3_fmaxf( node->aabbMax, tri.vertices[1] );
        node->aabbMax = vec3_fmaxf( node->aabbMax, tri.vertices[2] );
    }

    static glm::vec3 vec3_fmaxf(const glm::vec3& a, const glm::vec3& b) {
        return glm::vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    }

    static glm::vec3 vec3_fminf(const glm::vec3& a, const glm::vec3& b) {
        return glm::vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    }
};

#endif // MESHLOADER_H
