#include "InputManager.hpp"
#include "InputAction.hpp"

void InputManager::addControlElement(const std::string& name, const std::unordered_set<InputType>& allowed) {
    ctrls_[name] = std::make_shared<ControlElement>(name, allowed);
}

std::shared_ptr<ControlElement> InputManager::getControlElement(const std::string& name) const {
    auto it = ctrls_.find(name);
    return it != ctrls_.end() ? it->second : nullptr;
}

void InputManager::addKeyboardAction(const std::string& key, double time, ActionType at, const std::string& control_name) {
    auto c = getControlElement(control_name);
    if (c) queue_.addAction(std::make_unique<KeyboardInput>(key, time, at, c));
}

void InputManager::addMouseAction(int k, double time, ActionType at, const std::string& control_name) {
    auto c = getControlElement(control_name);
    if (c) queue_.addAction(std::make_unique<MouseInput>(k, time, at, c));
}

void InputManager::addTouchAction(double time, std::vector<FingerCoordinates> cords, const std::string& control_name) {
    auto c = getControlElement(control_name);
    if (c) queue_.addAction(std::make_unique<TouchInput>(time, std::move(cords), c));
}

std::vector<std::string> InputManager::performActions(double now) {
    return queue_.performActions(now);
}
