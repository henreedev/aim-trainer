#ifndef NAVMESH_H
#define NAVMESH_H

#include "Engine/triangle.h"
#include <queue>
#include <unordered_map>
#include <vector>
#include <limits>

class NavMesh {
private:

    float distance(const glm::vec3& p1, const glm::vec3& p2) {
        return glm::length(p1 - p2);
    }

    float heuristic(const glm::vec3& p1, const glm::vec3& p2) {
        return distance(p1, p2);
    }

public:
    NavMesh(){}
    std::vector<Triangle> triangles;

    void addTriangle(const Triangle& triangle) {
        triangles.push_back(triangle);
    }

    void generateNeighbors() {
        for (Triangle& triangle : triangles) {
            for (Triangle& otherTriangle : triangles) {
                if (&triangle != &otherTriangle && shareEdge(triangle, otherTriangle)) {
                    // if they share an edge the average of their points is the midpoint (from top-down at least)
                    glm::vec3 midpoint = (triangle.vertices[0] + triangle.vertices[1] +
                                          triangle.vertices[2] + otherTriangle.vertices[0] +
                                          otherTriangle.vertices[1] + otherTriangle.vertices[2]) / 6.0f;
                    triangle.neighbors.push_back(std::make_pair(&otherTriangle, midpoint));
                }
            }
        }
    }

    Triangle* findClosestTriangle(const glm::vec3& point) {
        Triangle* closestTriangle = nullptr;
        float minDistance = std::numeric_limits<float>::max();

        for (Triangle& triangle : triangles) {
            glm::vec3 centroid = (triangle.vertices[0] + triangle.vertices[1] + triangle.vertices[2]) / 3.0f;
            float distanceToTriangle = glm::distance(point, centroid);

            if (distanceToTriangle < minDistance) {
                minDistance = distanceToTriangle;
                closestTriangle = &triangle;
            }
        }

        return closestTriangle;
    }

    // A*
    std::vector<glm::vec3> findPath(const glm::vec3& start, const glm::vec3& end) {
        std::vector<glm::vec3> path;

        std::priority_queue<std::pair<float, Triangle*>, std::vector<std::pair<float, Triangle*>>, std::greater<std::pair<float, Triangle*>>> open;
        std::unordered_map<Triangle*, Triangle*> parent;
        std::unordered_map<Triangle*, float> cost;

        Triangle* startTriangle = findClosestTriangle(start);
        open.push({0, startTriangle});
        cost[startTriangle] = 0;

        while (!open.empty()) {
            Triangle* currentTriangle = open.top().second;
            open.pop();

            if (currentTriangle == findClosestTriangle(end)) {
                Triangle* node = currentTriangle;
                path.push_back(end);
                while (node) {
                    for (auto& neighbor : node->neighbors) {
                        if (neighbor.first == parent[node]) {
                            path.push_back(neighbor.second);
                            break;
                        }
                    }
                    node = parent[node];
                }
                std::reverse(path.begin(), path.end());
                break;
            }

            for (auto& neighbor : currentTriangle->neighbors) {
                Triangle* neighborTriangle = neighbor.first;
                glm::vec3 midpoint = neighbor.second;

                float newCost = cost[currentTriangle] + distance(currentTriangle->vertices[0], midpoint);

                if (!cost.count(neighborTriangle) || newCost < cost[neighborTriangle]) {

                    cost[neighborTriangle] = newCost;

                    float h = heuristic(neighborTriangle->vertices[0], end);
                    float totalCost = newCost + h;

                    open.push({totalCost, neighborTriangle});

                    parent[neighborTriangle] = currentTriangle;
                }
            }
        }
        return path;
    }

private:
    bool shareEdge(const Triangle& triangle1, const Triangle& triangle2) {
        int sharedVertices = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (glm::all(glm::equal(triangle1.vertices[i], triangle2.vertices[j]))) {
                    sharedVertices++;
                }
            }
        }
        return sharedVertices == 2;
    }
};


#endif // NAVMESH_H
