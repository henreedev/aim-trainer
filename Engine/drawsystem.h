#ifndef DRAWSYSTEM_H
#define DRAWSYSTEM_H

#include "Engine/System.h"

class DrawSystem : public System {
public:
    DrawSystem();
    ~DrawSystem() {}
    void draw();
};

#endif // DRAWSYSTEM_H
