#pragma once
#include <vector>
#include <limits>
#include "InputAction.hpp"
#include "LeftistHeap.hpp"


class ActionsQueue {
public:
    ActionsQueue() = default;
    ActionsQueue(const ActionsQueue& other) = default;
    ActionsQueue& operator=(const ActionsQueue& other) = default;

    void addAction(std::unique_ptr<InputAction> a);
    void performActions(double now);
    std::vector<const InputAction*> getActionsForControl(const std::string& control_name, double start = 0.0, double end = std::numeric_limits<double>::max()) const;
private:
    LeftistHeap heap_;
};
