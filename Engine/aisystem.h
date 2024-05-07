#ifndef AISYSTEM_H
#define AISYSTEM_H

#include "Engine/System.h"
#include "Engine/navmesh.h"

class AiSystem : public System
{
public:
    AiSystem();
    void update(float deltaTime);
    void reset(){}
    float m_timer;
    static std::shared_ptr<GameObject> player;
    static NavMesh* navMesh;
};

#endif // AISYSTEM_H
