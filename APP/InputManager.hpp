#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "ControlElement.hpp"
#include "ActionsQueue.hpp"

class InputManager {
public:
    void addControlElement(const std::string& name, const std::unordered_set<InputType>& allowed);
    std::shared_ptr<ControlElement> getControlElement(const std::string& name) const;
    void addKeyboardAction(const std::string& key, double time, ActionType at, const std::string& control_name);
    void addMouseAction(int k, double time, ActionType at, const std::string& control_name);
    void addTouchAction(double time, std::vector<FingerCoordinates> v, const std::string& control_name);
    std::vector<std::string> performActions(double now);
private:
    std::unordered_map<std::string, std::shared_ptr<ControlElement>> ctrls_;
    ActionsQueue queue_;
};
