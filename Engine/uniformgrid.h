#ifndef UNIFORMGRID_H
#define UNIFORMGRID_H


#include "glm/ext/vector_float3.hpp"
#include <memory>
#include <vector>
class GameObject;

class UniformGrid
{
public:
    UniformGrid(glm::vec3 dimsMin,
                glm::vec3 dimsMax, int subdivisions);
    // Places provided gameObjects into grid locations based on position and scale
    // A gameObject can end up in multiple grid locations
    void update(std::vector<std::shared_ptr<GameObject>> gObjs);
    glm::vec3 dimsMin;
    glm::vec3 dimsMax;
    glm::vec3 dimsDiff;
    std::vector<std::vector<std::vector<std::vector<GameObject>>>> grid;
    int size;
};

#endif // UNIFORMGRID_H
