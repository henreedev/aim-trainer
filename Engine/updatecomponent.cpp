#include "updatecomponent.h"

UpdateComponent::UpdateComponent(std::weak_ptr<GameObject> parent, std::function<void(float)> updateFunc) : m_updateFunc(updateFunc)
{
    m_parent = parent;
}


void UpdateComponent::update(float deltaTime)
{
    if (m_updateFunc) m_updateFunc(deltaTime);
}
