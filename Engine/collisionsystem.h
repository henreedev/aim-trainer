#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "Engine/uniformgrid.h"
#include "System.h"


class CollisionSystem : public System
{
public:
    CollisionSystem();
    CollisionSystem(UniformGrid& uniformGrid);
    void update(float deltaTime);
    void addGameObject(std::shared_ptr<GameObject> gameObject) override;
    void removeGameObject(std::shared_ptr<GameObject> gameObject) override;

private:
    std::vector<std::shared_ptr<GameObject>> m_staticObjs;
    std::vector<std::shared_ptr<GameObject>> m_dynamicObjs;
    UniformGrid m_uniformGrid;
};

#endif // COLLISIONSYSTEM_H
