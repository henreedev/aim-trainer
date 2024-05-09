#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "Engine/aicomponent.h"
#include "Engine/aisystem.h"
#include "Engine/camerasystem.h"
#include "Engine/charactercontrollersystem.h"
#include "Engine/collisioncomponent.h"
#include "Engine/collisionsystem.h"
#include "Engine/drawsystem.h"
#include "Engine/updatesystem.h"
#include "charactercontrollercomponent.h"
#include "drawcomponent.h"
#include <iostream>
#include <map>
#include <memory>
#include <typeindex>
#include <vector>

class Screen;
class GameObject;
class System;
class CharacterControllerComponent;

class DrawComponent;
class DrawSystem;
class CameraComponent;
class CameraSystem;
class CollisionSystem;



class GameWorld {
public:
    static std::shared_ptr<GameWorld> gameWorldInstance;

    void addGameObject(std::shared_ptr<GameObject> gameObject) {
        m_objects.push_back(gameObject);
        if (gameObject->hasComponent<CharacterControllerComponent>()) {
            if (hasSystem<CharacterControllerSystem>()) {
                getSystem<CharacterControllerSystem>()->addGameObject(gameObject);
            }
        }
        if (gameObject->hasComponent<CameraComponent>()) {
            if (hasSystem<CameraSystem>()) {
                getSystem<CameraSystem>()->addGameObject(gameObject);
            }
        }
        if (gameObject->hasComponent<DrawComponent>()) {
            if (hasSystem<DrawSystem>()) {
                getSystem<DrawSystem>()->addGameObject(gameObject);
            }
        }

        if (gameObject->hasComponent<CollisionComponent>()) {
            if (hasSystem<CollisionSystem>()) {
                getSystem<CollisionSystem>()->addGameObject(gameObject);
            }
        }
        if (gameObject->hasComponent<AiComponent>()) {
            if (hasSystem<AiSystem>()) {
                getSystem<AiSystem>()->addGameObject(gameObject);
            }
        }
        if (gameObject->hasComponent<ScenarioComponent>()) {
            if (hasSystem<UpdateSystem>()) {
                getSystem<UpdateSystem>()->addGameObject(gameObject);
            }
        }

    }

    void removeGameObject(std::shared_ptr<GameObject> gameObject) {
        m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), gameObject), m_objects.end());
        if (gameObject->hasComponent<CharacterControllerComponent>()) {
            if (hasSystem<CharacterControllerSystem>()) {
                getSystem<CharacterControllerSystem>()->removeGameObject(gameObject);
            }
        }
        if (gameObject->hasComponent<CameraComponent>()) {
            if (hasSystem<CameraSystem>()) {
                getSystem<CameraSystem>()->removeGameObject(gameObject);
            }
        }
        if (gameObject->hasComponent<DrawComponent>()) {
            if (hasSystem<DrawSystem>()) {
                getSystem<DrawSystem>()->removeGameObject(gameObject);
            }
        }

        if (gameObject->hasComponent<CollisionComponent>()) {
            if (hasSystem<CollisionSystem>()) {
                getSystem<CollisionSystem>()->removeGameObject(gameObject);
            }
        }
        if (gameObject->hasComponent<AiComponent>()) {
            if (hasSystem<AiSystem>()) {
                getSystem<AiSystem>()->removeGameObject(gameObject);
            }
        }
        if (gameObject->hasComponent<ScenarioComponent>()) {
            if (hasSystem<UpdateSystem>()) {
                getSystem<UpdateSystem>()->removeGameObject(gameObject);
            }
        }

    }


    std::shared_ptr<GameObject> getGameObject(int index) {
        return m_objects[index];
    }

    template <class T>
    void addSystem(std::shared_ptr<T> system) {
        std::shared_ptr<System> castedSystem = std::dynamic_pointer_cast<System>(system);
        m_systems[typeid(T)] = castedSystem;
    }

    template <class T>
    std::shared_ptr<T> getSystem() {
        if (!m_systems.count(typeid(T))) {
            throw std::invalid_argument(std::string("getSystem() tried to retrieve nonexistent System type: ") + typeid(T).name());
        }
        std::shared_ptr<System> system = m_systems[typeid(T)];
        return std::static_pointer_cast<T>(system);
    }

    template <class T>
    bool hasSystem() {
        return m_systems.count(typeid(T));
    }

    template <class T>
    void removeSystem() {
        size_t res = m_systems.erase(typeid(T));
        if (res == 0) {
            std::cerr << "Warning: nonexistent System removed: " << typeid(T).name() << std::endl;
        }
    }

    void update(double deltaTime){
        // Character Controller system
        if (hasSystem<CharacterControllerSystem>()) getSystem<CharacterControllerSystem>()->update(deltaTime);
        // Collision system
        if (hasSystem<CollisionSystem>()) getSystem<CollisionSystem>()->update(deltaTime);
        // Camera system
        if (hasSystem<CameraSystem>()) getSystem<CameraSystem>()->update(deltaTime);
        // AI system
        if (hasSystem<AiSystem>()) getSystem<AiSystem>()->update(deltaTime);
        // Generic update system
        if (hasSystem<UpdateSystem>()) getSystem<UpdateSystem>()->update(deltaTime);


    }

    void draw(){
        // Draw system
        if (hasSystem<DrawSystem>()) getSystem<DrawSystem>()->draw();
    }

private:
    std::vector<std::shared_ptr<GameObject>> m_objects;
    std::map<std::type_index, std::shared_ptr<System>> m_systems;
};



#endif // GAMEWORLD_H
