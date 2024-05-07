#include "transformcomponent.h"

TransformComponent::TransformComponent(std::weak_ptr<GameObject> parent, glm::vec3 pos, float scale)
{
    m_parent = parent;
    m_model = std::make_shared<ModelTransform>();
    m_model->setPos(pos);
    m_model->setScale(scale);
}

TransformComponent::TransformComponent(std::weak_ptr<GameObject> parent, std::shared_ptr<ModelTransform> transform)
{
    m_parent = parent;
    m_model = transform;
}

void TransformComponent::setPos(glm::vec3 pos) {
    m_model->setPos(pos);
}
void TransformComponent::translate(glm::vec3 delta) {
    m_model->translate(delta);
}
glm::vec3 TransformComponent::getPos() {
    return m_model->getPos();
}
void TransformComponent::setScale(glm::vec3 scale) {
    m_model->setScale(scale);
}
void TransformComponent::setScale(float scale) {
    m_model->setScale(scale);
}
void TransformComponent::scale(glm::vec3 scale) {
    m_model->scale(scale);
}
void TransformComponent::scale(float scale) {
    m_model->scale(scale);
}
glm::vec3 TransformComponent::getScale() {
    return m_model->getScale();
}
void TransformComponent::setRotation(float angle, glm::vec3 axis) {
    m_model->setRotation(angle, axis);
}
void TransformComponent::setRotation(glm::mat4 r) {
    m_model->setRotation(r);
}
void TransformComponent::rotate(float angle, glm::vec3 axis) {
    m_model->rotate(angle, axis);
}
void TransformComponent::rotate(glm::mat4 r) {
    m_model->rotate(r);
}
glm::mat4 TransformComponent::getRotation() {
    return m_model->getRotation();
}
glm::mat4 TransformComponent::getModelMatrix() {
    return m_model->getModelMatrix();
}
void TransformComponent::clear() {
    m_model->clear();
}

