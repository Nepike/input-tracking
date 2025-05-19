#include "ControlElement.hpp"

ControlElement::ControlElement(const std::string& name, const std::unordered_set<InputType>& allowed)
    : name_(name), allowed_(allowed) {}

void ControlElement::addAction(InputType t) {
    allowed_.insert(t);
}

void ControlElement::removeAction(InputType t) {
    allowed_.erase(t);
}

bool ControlElement::checkAction(InputType t) const {
    return allowed_.count(t);
}

const std::string& ControlElement::getName() const {
    return name_;
}
