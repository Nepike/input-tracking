#pragma once
#include <string>
#include <unordered_set>

enum class InputType { KEYBOARD, MOUSE, TOUCH };

class ControlElement {
public:
    ControlElement(const std::string& name, const std::unordered_set<InputType>& allowed);
    void addAction(InputType t);
    void removeAction(InputType t);
    bool checkAction(InputType t) const;
    const std::string& getName() const;

private:
    std::string name_;
    std::unordered_set<InputType> allowed_;
};
