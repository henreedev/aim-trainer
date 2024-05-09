#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "component.h"
#include <raudio.h>

class GameWorld;

class HealthComponent : public Component
{
public:
    HealthComponent(std::weak_ptr<GameObject> parent_weak, float health, float maxHealth)
        : health(health), maxHealth(maxHealth)
    {
        m_parent = parent_weak;
        sound = LoadMusicStream("Resources/resources/pistol.mp3");
    }

    void takeDamage(float damage) {
        health -= damage;
        if (health <= 0) {
            die();
        }
    }
    void die();
    void setHealth(float newHealth) {
        health = newHealth;
    }
    float getHealth() {
        return health;
    }
    float health;
    float maxHealth;
    Music sound;
};

#endif // HEALTHCOMPONENT_H
