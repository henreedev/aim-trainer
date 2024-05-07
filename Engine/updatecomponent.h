#ifndef UPDATECOMPONENT_H
#define UPDATECOMPONENT_H


#include "Engine/component.h"
#include <functional>
class UpdateComponent : public Component
{
public:
    UpdateComponent(std::weak_ptr<GameObject> parent, std::function<void(float)>);
    void update(float deltaTime);

private:
    std::function<void(float)> m_updateFunc;
};

#endif // UPDATECOMPONENT_H
