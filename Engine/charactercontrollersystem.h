#ifndef CHARACTERCONTROLLERSYSTEM_H
#define CHARACTERCONTROLLERSYSTEM_H


#include "Engine/System.h"

class CharacterControllerSystem : public System
{
public:
    CharacterControllerSystem(float gravity = 5.0f);
    void update(float deltaTime);
    float m_gravity;
};

#endif // CHARACTERCONTROLLERSYSTEM_H
