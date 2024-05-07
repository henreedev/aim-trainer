#ifndef SYSTEM_H
#define SYSTEM_H

#include <memory>
#include <vector>
class GameObject;

class System {
public:
    virtual ~System() {}
    virtual void addGameObject(std::shared_ptr<GameObject> gameObject) {
        m_objects.push_back(gameObject);
    }
    virtual void removeGameObject(std::shared_ptr<GameObject> gameObject) {
        m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), gameObject), m_objects.end());
    }
    std::shared_ptr<GameObject> getGameObject(int index) {
        return m_objects[index];
    }
protected:
    std::vector<std::shared_ptr<GameObject>> m_objects;
};

#endif // SYSTEM_H
