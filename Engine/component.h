#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

#include <memory>

class Component {
public:
    virtual ~Component() {};
    std::weak_ptr<GameObject> getParent(){ return m_parent;}
protected:
    std::weak_ptr<GameObject> m_parent;
};


#endif // COMPONENT_H
