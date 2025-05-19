#include "InputAction.hpp"

InputAction::InputAction(double time, std::shared_ptr<ControlElement> ctrl)
    : time_(time), ctrl_(std::move(ctrl)) {}

bool InputAction::operator<(const InputAction& other) const {
    return time_ < other.time_;
}

bool InputAction::operator==(const InputAction& other) const {
    return time_ == other.time_;  
}

double InputAction::getTime() const {
    return time_;
}

std::shared_ptr<ControlElement> InputAction::getControl() const {
    return ctrl_;
}


// --- KeyboardInput ---
KeyboardInput::KeyboardInput(std::string key, double time, ActionType at, std::shared_ptr<ControlElement> c)
    : InputAction(time, c), key_(std::move(key)), actionType_(at) {}

// KeyboardInput KeyboardInput::createRelease(const KeyboardInput& pressAction, double time) {
//     return KeyboardInput(pressAction.key_, time, ActionType::RELEASE, pressAction.getControl());
// }

bool KeyboardInput::perform(double current) const {
    auto ctrl = getControl();
    return ctrl && ctrl->checkAction(InputType::KEYBOARD) && getTime() <= current;
}

std::unique_ptr<InputAction> KeyboardInput::clone() const {
    return std::make_unique<KeyboardInput>(*this);
}


// --- MouseInput ---
MouseInput::MouseInput(int key, double time, ActionType at, std::shared_ptr<ControlElement> c)
    : InputAction(time, c), keyNum_(key), actionType_(at) {}

bool MouseInput::perform(double current) const {
    auto ctrl = getControl();
    return ctrl && ctrl->checkAction(InputType::MOUSE) && getTime() <= current;
}

std::unique_ptr<InputAction> MouseInput::clone() const {
    return std::make_unique<MouseInput>(*this);
}


// --- TouchInput ---
TouchInput::TouchInput(double time, std::vector<FingerCoordinates> cords, std::shared_ptr<ControlElement> c)
    : InputAction(time, c), cords_(std::move(cords)) {}

bool TouchInput::perform(double current) const {
    auto ctrl = getControl();
    return ctrl && ctrl->checkAction(InputType::TOUCH) && getTime() <= current;
}

std::unique_ptr<InputAction> TouchInput::clone() const {
    return std::make_unique<TouchInput>(*this);
}
