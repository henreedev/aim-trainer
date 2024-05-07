#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

#include <memory>

class Component {
public:
    virtual ~Component() {};
protected:
    std::weak_ptr<GameObject> m_parent;
};


#endif // COMPONENT_H
