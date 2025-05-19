#pragma once
#include <memory>
#include <vector>
#include "ControlElement.hpp"

enum class ActionType { PRESS, RELEASE };

class InputAction {
public:
    InputAction(double time, std::shared_ptr<ControlElement> ctrl);
    virtual ~InputAction() = default;

    bool operator<(const InputAction& other) const;
    bool operator==(const InputAction& other) const;
    double getTime() const;
    std::shared_ptr<ControlElement> getControl() const;
    virtual bool perform(double current) const = 0;
    virtual std::unique_ptr<InputAction> clone() const = 0;

private:
    double time_;
    std::shared_ptr<ControlElement> ctrl_;
};


class KeyboardInput : public InputAction {
public:
    KeyboardInput(std::string key, double time, ActionType at, std::shared_ptr<ControlElement> c);
    //static KeyboardInput createRelease(const KeyboardInput& pressAction, double time); // (unnecessary)
    bool perform(double current) const override;
    std::unique_ptr<InputAction> clone() const override;
private:
    std::string key_;
    ActionType actionType_;
};


class MouseInput : public InputAction {
public:
    MouseInput(int key, double time, ActionType at, std::shared_ptr<ControlElement> c);
    bool perform(double current) const override;
    std::unique_ptr<InputAction> clone() const override;
private:
    int keyNum_;
    ActionType actionType_;
};


struct FingerCoordinates {
    double sx, sy, ex, ey; // (start_x start_y), (end_x end_y) 
};

class TouchInput : public InputAction {
public:
    TouchInput(double time, std::vector<FingerCoordinates> cords, std::shared_ptr<ControlElement> c);
    bool perform(double current) const override;
    std::unique_ptr<InputAction> clone() const override;
private:
    std::vector<FingerCoordinates> cords_;
};
