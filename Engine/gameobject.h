#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <map>
#include <queue>
#include <memory>
#include <typeindex>
#include <typeinfo>

class Component;

class GameObject {
public:
    template <class T>
    void addComponent(std::shared_ptr<T> component) {
        std::shared_ptr<Component> castedComponent = std::dynamic_pointer_cast<Component>(component);
        m_components[typeid(T)] = castedComponent;
    }

    template <class T>
    std::shared_ptr<T> getComponent() {
        if (!m_components.count(typeid(T))) {
            throw std::invalid_argument(std::string("getComponent() tried to retrieve nonexistent component type: ") + typeid(T).name());
        }
        std::shared_ptr<Component> component = m_components[typeid(T)];
        return std::static_pointer_cast<T>(component);
    }

    template <class T>
    bool hasComponent() {
        return m_components.count(typeid(T));
    }

    template <class T>
    void removeComponent() {
        size_t res = m_components.erase(typeid(T));
        if (res == 0) {
            std::cerr << "Warning: nonexistent component removed: " << typeid(T).name() << std::endl;
        }
    }

private:
    std::map<std::type_index, std::shared_ptr<Component>> m_components;
};


#endif // GAMEOBJECT_H
