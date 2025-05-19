#include "ActionsQueue.hpp"

void ActionsQueue::addAction(std::unique_ptr<InputAction> a) {
    heap_.insert(std::move(a));
}

void ActionsQueue::performActions(double now) {
    while (!heap_.empty() && heap_.peekMin()->perform(now)) {
        heap_.extractMin();
    }
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