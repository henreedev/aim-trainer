#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "component.h"
class BTNode;

class AiComponent : public Component
{
private:
    std::weak_ptr<GameObject> m_parent;
    BTNode* behaviorTree;
public:
    AiComponent(std::weak_ptr<GameObject> parent, BTNode* behaviorTree) : m_parent(parent), behaviorTree(behaviorTree)
        {};
    void reset();
    void update(float deltaTime);
};

#endif // AICOMPONENT_H
