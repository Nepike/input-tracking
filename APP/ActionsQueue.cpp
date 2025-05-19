#include "ActionsQueue.hpp"

void ActionsQueue::addAction(std::unique_ptr<InputAction> a) {
    heap_.insert(std::move(a));
}

std::vector<std::string> ActionsQueue::performActions(double now) {
    LeftistHeap left;
    std::vector<std::string> responses;
    std::string response;

    while (!heap_.empty()) {
        auto action = heap_.extractMin();
        response = action->perform(now);

        if (response != "") {
            responses.push_back(response);
        }
        else {
            left.insert(std::move(action));
        }
    }
    heap_ = std::move(left); // Кажется, это корректно, учитывая конструкторы в LeftistHeap
    return responses;
}

std::vector<const InputAction*> ActionsQueue::getActionsForControl(const std::string& control_name, double start, double end) const {
    std::vector<const InputAction*> result;
    heap_.traverse([&](const InputAction* act){
            if (act->getControl()->getName() == control_name) {
                double t = act->getTime();
                if (t >= start && t <= end) {
                    result.push_back(act);
                }
            }
        });
    return result;
}