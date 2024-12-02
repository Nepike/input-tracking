#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include <memory>


enum class InputType {
  KeyboardInput,
  MouseInput,
  TouchInput,
};

enum class ActionType {
  Press,
  Release,
};

class ControlElement {
  public:
    ControlElement(const std::string& name, const std::set<InputType>& actions) : name_(name), actions_(actions) {}

    void addAction(InputType action) {
      actions_.insert(action);
    }

    bool reactsTo(InputType input) const {
      return actions_.find(input) != actions_.end();
    }

    const std::string& getName() const {
      return name_;
    }

  private:
    std::string name_;
    std::set<InputType> actions_; // Набор действий, на которые реагирует элемент
};

class Input {
  public:
    virtual ~Input() = default;
    
    bool operator<(const Input& other) const {
      return actionTime_ < other.actionTime_;
    }

    bool operator==(const Input& other) const {
      return actionTime_ == other.actionTime_;  
    }

    const ControlElement* getControl() const {
      return controlElement_;
    }

    double getActionTime() const {
      return actionTime_;
    }

    virtual void executeAction() const = 0;

    virtual void print() const = 0;


  protected:
    double actionTime_;
    ControlElement* controlElement_;

};

class KeyboardInput: public Input {
  public:
    KeyboardInput(const std::string& keyName, double actionTime, ActionType actionType, ControlElement* controlElement) {
      keyName_ = keyName;
      actionTime_ = actionTime;
      actionType_ = actionType;
      controlElement_ = controlElement;
    }

    static KeyboardInput createRelease(const KeyboardInput& pressAction, double releaseTime){
      return KeyboardInput(pressAction.keyName_, releaseTime, ActionType::Release, pressAction.controlElement_);
    }

    void executeAction() const override {
      if (controlElement_ && controlElement_->reactsTo(InputType::KeyboardInput)) {
        std::cout << "Keyboard action executed: " << keyName_ << " at time " << actionTime_ << "\n";
      }
    }

    void print() const override {
      std::cout << "KeyboardInput: KeyName=" << keyName_ << ", ActionTime=" << actionTime_ << ", ActionType=" << (actionType_ == ActionType::Press ? "Press" : "Release") << "\n";
    }

  private:
    std::string keyName_;
    ActionType actionType_;
};

class MouseInput: public Input {
  public:
    MouseInput(int keyNumber, double actionTime, ActionType actionType, ControlElement* controlElement) {
      keyNumber_ = keyNumber;
      actionTime_ = actionTime;
      actionType_ = actionType;
      controlElement_ = controlElement;
    }

    void executeAction() const override {
      if (controlElement_ && controlElement_->reactsTo(InputType::MouseInput)) {
        std::cout << "Mouse action executed: Button=" << keyNumber_ << " at time " << actionTime_ << "\n";
      }
    }

    void print() const override {
      std::cout << "MouseInput: Button=" << keyNumber_ << ", ActionTime=" << actionTime_ << ", ActionType=" << (actionType_ == ActionType::Press ? "Press" : "Release") << "\n";
    }

  private:
    int keyNumber_;
    ActionType actionType_;
};

struct FingerCoordinates {
  FingerCoordinates(double sx, double sy, double ex, double ey) {
    start.first = sx;
    start.second = sy;
    end.first = ex;
    end.second = ey;
  }
  std::pair<double, double> start;
  std::pair<double, double> end;
};

class TouchInput: public Input {
  public:
    TouchInput(double actionTime, const std::vector<FingerCoordinates>& fingersCords,  ControlElement* controlElement) {
      actionTime_ = actionTime;
      controlElement_ = controlElement;
      fingersCords_ = fingersCords;
      fingersCount_ = fingersCords.size();
    }

    void executeAction() const override {
      if (controlElement_ && controlElement_->reactsTo(InputType::TouchInput)) {
        std::cout << "Touch action executed with " << fingersCount_ << " fingers at time " << actionTime_ << "\n";
      }
    }

    void print() const override {
      std::cout << "TouchInput: ActionTime=" << actionTime_ << ", FingersCount=" << fingersCount_ << ", Coordinates=";
      for (const auto& coord : fingersCords_) {
        std::cout << " [Start(" << coord.start.first << ", " << coord.start.second << ") -> End(" << coord.end.first << ", " << coord.end.second << ")]";
      }
      std::cout << "\n";
    }

  private:
    std::vector<FingerCoordinates> fingersCords_;
    int fingersCount_;
};

class ActionsQueue {
  public:
    ActionsQueue(): actions_({}) {}

    void addAction(std::shared_ptr<Input> action) {
      actions_.push_back(action);
      std::sort(actions_.begin(), actions_.end(), [](const std::shared_ptr<Input>& a, const std::shared_ptr<Input>& b) {
        return a->getActionTime() < b->getActionTime();
      });
    }

    std::vector<std::shared_ptr<Input>> getActionsForControl(const std::string& controlName, double startTime = 0.0, double endTime = std::numeric_limits<double>::max()) const {
      std::vector<std::shared_ptr<Input>> result;
      for (const auto& action : actions_) {
        if (action->getControl()->getName() == controlName && action->getActionTime() >= startTime && action->getActionTime() <= endTime) {
          result.push_back(action);
        }
      }
      return result;
    }

    void executeActions() const {
      std::cout << std::endl;
      for (const auto& action : actions_) {
        action->executeAction();
      }
    }

  private:
    std::vector<std::shared_ptr<Input>> actions_;
};



int main() {
  // Test ControlElement class
  ControlElement button("PlayButton", {InputType::KeyboardInput, InputType::MouseInput});
  assert(button.getName() == "PlayButton");
  assert(button.reactsTo(InputType::KeyboardInput) == true);
  assert(button.reactsTo(InputType::TouchInput) == false);
  button.addAction(InputType::TouchInput);
  assert(button.reactsTo(InputType::TouchInput) == true);

  // Test KeyboardInput
  ControlElement keyboardControl("KeyboardControl", {InputType::KeyboardInput});
  KeyboardInput keyPress("A", 1.0, ActionType::Press, &keyboardControl);
  KeyboardInput keyRelease = KeyboardInput::createRelease(keyPress, 2.0);
  assert(keyPress.getControl()->getName() == "KeyboardControl");
  assert(keyPress.getActionTime() == 1.0);
  assert(keyRelease.getActionTime() == 2.0);

  // Test MouseInput
  ControlElement mouseControl("MouseControl", {InputType::MouseInput});
  MouseInput mousePress(1, 1.5, ActionType::Press, &mouseControl);
  assert(mousePress.getControl()->getName() == "MouseControl");
  assert(mousePress.getActionTime() == 1.5);

  // Test TouchInput
  ControlElement touchControl("TouchControl", {InputType::TouchInput});
  std::vector<FingerCoordinates> fingers = {
      FingerCoordinates(0.0, 0.0, 1.0, 1.0),
      FingerCoordinates(1.0, 1.0, 2.0, 2.0)
  };
  TouchInput touch(2.5, fingers, &touchControl);
  assert(touch.getControl()->getName() == "TouchControl");
  assert(touch.getActionTime() == 2.5);

  // Test polymorphism
  std::vector<std::shared_ptr<Input>> inputs;
  inputs.push_back(std::make_shared<KeyboardInput>(keyPress));
  inputs.push_back(std::make_shared<MouseInput>(mousePress));
  inputs.push_back(std::make_shared<TouchInput>(touch));

  for (const auto& input : inputs) {
      input->print();
  }

  // Test ActionsQueue
  ActionsQueue actionQueue;
  actionQueue.addAction(std::make_shared<KeyboardInput>(keyPress));
  actionQueue.addAction(std::make_shared<MouseInput>(mousePress));
  actionQueue.addAction(std::make_shared<TouchInput>(touch));

  std::vector<std::shared_ptr<Input>> retrievedActions = actionQueue.getActionsForControl("MouseControl", 0.0, 2.0);
  assert(retrievedActions.size() == 1);
  assert(retrievedActions[0]->getActionTime() == 1.5);

  actionQueue.executeActions();

  std::cout << "All tests passed!" << std::endl;

  return 0;
}





