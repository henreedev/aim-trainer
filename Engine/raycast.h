#ifndef RAYCAST_H
#define RAYCAST_H

#include "Graphics/global.h"
#include <memory>
#include "Engine/ellipsecollider.h"
#include "Engine/gameobject.h"
#include "Engine/healthcomponent.h"
#include "Engine/transformcomponent.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "triangle.h"
#include <raudio.h>

class GameObject;

class Ray {
public:
    Sound pistol;
    Sound metal;
    float minT = 999999.0f;
    std::weak_ptr<GameObject> closestObj = std::make_shared<GameObject>();
    glm::vec3 p;
    glm::vec3 d;
    bool isAutomaticWeapon = false;
    float dps = 100.0;
    void setWeaponAutomatic(bool isAutomatic) {isAutomaticWeapon = isAutomatic;}
    Ray(glm::vec3 p, glm::vec3 d) : p(p), d(d) { 
        InitAudioDevice();
        pistol = LoadSoundFromWave(LoadWave("Resources/resources/pistol.ogg"));
        metal = LoadSoundFromWave(LoadWave("Resources/resources/rico2.ogg"));
    }
    void reset(){minT = 999999.0f; closestObj = std::make_shared<GameObject>();}
    void intersect(std::shared_ptr<EllipseCollider> ellipsoid, float deltaTime) {
        if (Global::input.actionJustPressed(GLFW_MOUSE_BUTTON_LEFT)) PlaySound(pistol);
        if (ellipsoid->ignoreRaycast) return;
        float t0, t1; // Solutions for t if the ray intersects the sphere
    
#if 0 \
    // Geometric solution
        Vec3f L = center - ray.orig;
        float tca = L.dotProduct(ray.dir);
        // if (tca < 0) return false;
        float d2 = L.dotProduct(L) - tca * tca;
        if (d2 > radius * radius) return false;
        float thc = sqrt(radius * radius - d2);
        t0 = tca - thc;
        t1 = tca + thc;
#else \
    // Analytic solution
        glm::vec3 center = ellipsoid->m_transform->getPos();
        float radius = ellipsoid->m_transform->getScale().x / 2.0f;
        glm::vec3 L = p - center;
        float a = glm::dot(d, d);
        float b = 2 * glm::dot(d,L);
        float c = glm::dot(L, L) - radius * radius;
        if (!Triangle::solveQuadratic(a, b, c, t0, t1)) return;
#endif
        if (t0 > t1) std::swap(t0, t1);

        if (t0 < 0) {
            t0 = t1; // If t0 is negative, let's use t1 instead.
            if (t0 < 0) return; // Both t0 and t1 are negative.
        }
        if (t0 >= 0.0 && t0 < minT) {
            minT = t0;
            closestObj = ellipsoid->m_transform->getParent();
            if (!closestObj.expired()) {
                if (closestObj.lock()->hasComponent<HealthComponent>()) {
                    if (isAutomaticWeapon) {
                        if (Global::input.actionHeld(GLFW_MOUSE_BUTTON_LEFT)) {
                            closestObj.lock()->getComponent<HealthComponent>()->takeDamage(dps * deltaTime);
                        }
                    } else {
                        if (Global::input.actionJustPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                            PlaySound(metal);
                            
                            closestObj.lock()->getComponent<HealthComponent>()->takeDamage(dps);
                        }
                    }

                }
            }
        }
    }
private:

};

#endif // RAYCAST_H
