#ifndef BEHAVIORTREE_H
#define BEHAVIORTREE_H

#include <functional>
#include <map>
#include <string>
#include <vector>
enum BTStatus {
    SUCCESS, RUNNING, FAILURE
};

class BTNode {
public:
    virtual BTStatus update(float deltaTime) = 0;
    virtual void reset() = 0;
    static std::map<std::string, float> blackboard;
};

class Composite: public BTNode {
public:
    template<typename... Args>
    Composite(Args*... args) : m_children{args...} {}
    Composite(BTNode* child1, BTNode* child2) {m_children.push_back(child1); m_children.push_back(child2);}
    std::vector<BTNode *> m_children;
    BTNode *m_lastRunning;
};

class Action : public BTNode {
public:
    using UpdateFunction = std::function<BTStatus(float)>;

    // Constructor taking a lambda expression for custom update functionality
    Action(UpdateFunction updateFunc) : updateFunc(updateFunc) {}

    // Update function, delegates to the provided lambda
    BTStatus update(float deltaTime) override {
        return updateFunc(deltaTime);
    }
    void reset() override {};
private:
    UpdateFunction updateFunc;
};

class Condition : public BTNode {
public:
    using UpdateFunction = std::function<BTStatus(float)>;

    // Constructor taking a lambda expression for custom update functionality
    Condition(UpdateFunction updateFunc) : updateFunc(updateFunc) {}

    // Update function, delegates to the provided lambda
    BTStatus update(float deltaTime) override {
        return updateFunc(deltaTime);
    }
    void reset() override {};
private:
    UpdateFunction updateFunc;
};

class Selector : public Composite {
public:
    Selector(){}
    Selector(BTNode* child1, BTNode* child2) {m_children.push_back(child1); m_children.push_back(child2);}
    BTStatus update(float deltaTime) override {
        for(BTNode* child : m_children) {
            BTStatus childStatus = child->update(deltaTime);
            if(childStatus == RUNNING) {
                m_lastRunning = child;
                return RUNNING;
            } else if (childStatus == SUCCESS) {
                m_lastRunning = nullptr;
                return SUCCESS;
            }
        }
        m_lastRunning = nullptr;
        return FAILURE;
    }

    void reset() override {};
};

class Sequence : public Composite {
    BTStatus update(float deltaTime) override {
        auto it = std::find(m_children.begin(), m_children.end(), m_lastRunning);
        if (m_lastRunning != nullptr && it != m_children.end()) {
            for(int i = std::distance(m_children.begin(), it); i < m_children.size(); i++) {
                BTNode* child = m_children[i];
                BTStatus childStatus = child->update(deltaTime);
                if (childStatus == RUNNING) {
                    m_lastRunning = child;
                    return RUNNING;
                } else if (childStatus == FAILURE) {
                    m_lastRunning = nullptr;
                    return FAILURE;
                }
            }
        } else {
            for (BTNode* child : m_children) {
                BTStatus childStatus = child->update(deltaTime);
                if (childStatus == RUNNING) {
                    m_lastRunning = child;
                    return RUNNING;
                } else if (childStatus == FAILURE) {
                    m_lastRunning = nullptr;
                    return FAILURE;
                }
            }
        }
        m_lastRunning = nullptr;
        return SUCCESS;
    }

    void reset() override {

    }
};



#endif // BEHAVIORTREE_H
