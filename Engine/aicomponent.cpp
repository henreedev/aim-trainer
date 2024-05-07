#include "aicomponent.h"
#include "Engine/behaviortree.h"

void AiComponent::reset(){
    if(behaviorTree != nullptr) {
        behaviorTree->reset();
    }
}

void AiComponent::update(float deltaTime) {
    if(behaviorTree != nullptr) {
        behaviorTree->update(deltaTime);
    }
}
