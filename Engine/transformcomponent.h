#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "Engine/component.h"
#include "Graphics/modeltransform.h"
#include "glm/vec3.hpp"
#include <memory>



class TransformComponent : public Component
{
public:
    TransformComponent(std::weak_ptr<GameObject> parent, glm::vec3 pos, float scale);
    TransformComponent(std::weak_ptr<GameObject> parent, std::shared_ptr<ModelTransform> transform);

    // ModelTransform functions to pass down

    void setPos(glm::vec3 pos);
    void translate(glm::vec3 delta);
    glm::vec3 getPos();
    void setScale(glm::vec3 scale);
    void setScale(float scale);
    void scale(glm::vec3 scale);
    void scale(float scale);
    glm::vec3 getScale();
    void setRotation(float angle, glm::vec3 axis);
    void setRotation(glm::mat4 r);
    void rotate(float angle, glm::vec3 axis);
    void rotate(glm::mat4 r);
    glm::mat4 getRotation();
    glm::mat4 getModelMatrix();
    void clear();

private:
    std::shared_ptr<ModelTransform> m_model;
};

#endif // TRANSFORMCOMPONENT_H
