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

class InputAction {
  public:
    InputAction(double actionTime, const std::shared_ptr<ControlElement>& control): actionTime_(actionTime), controlElement_(control) {}
    virtual ~InputAction() = default;
    
    bool operator<(const InputAction& other) const {
      return actionTime_ < other.actionTime_;
    }

    bool operator==(const InputAction& other) const {
      return actionTime_ == other.actionTime_;  
    }

    const std::shared_ptr<ControlElement> getControl() const {
      return controlElement_;
    }

    double getActionTime() const {
      return actionTime_;
    }

    virtual void executeAction() const = 0;

    virtual void print() const = 0;

    virtual InputAction* clone() const = 0;


  private:
    double actionTime_;
    const std::shared_ptr<ControlElement> controlElement_;

};

class KeyboardInput: public InputAction {
  public:
    KeyboardInput(const std::string& keyName, double actionTime, ActionType actionType, const std::shared_ptr<ControlElement>& control)
    : InputAction(actionTime, control) {
      keyName_ = keyName;
      actionType_ = actionType;
    }

    static KeyboardInput createRelease(const KeyboardInput& pressAction, double releaseTime){
      return KeyboardInput(pressAction.keyName_, releaseTime, ActionType::Release, pressAction.getControl());
    }

    void executeAction() const override {
      std::shared_ptr<ControlElement> control = this->getControl();
      if (control && control->reactsTo(InputType::KeyboardInput)) {
        std::cout << "Keyboard action executed: " << keyName_ << " at time " << this->getActionTime() << "\n";
      }
    }

    void print() const override {
      std::cout << "KeyboardInput: KeyName=" << keyName_ << ", ActionTime=" << this->getActionTime() << ", ActionType=" << (actionType_ == ActionType::Press ? "Press" : "Release") << "\n";
    }

    KeyboardInput* clone() const override {
      return new KeyboardInput(keyName_, this->getActionTime(), actionType_, this->getControl());
    }

  private:
    std::string keyName_;
    ActionType actionType_;
};

class MouseInput: public InputAction {
  public:
    MouseInput(int keyNumber, double actionTime, ActionType actionType, const std::shared_ptr<ControlElement>& control)
    : InputAction(actionTime, control) {
      keyNumber_ = keyNumber;
      actionType_ = actionType;
    }

    void executeAction() const override {
      std::shared_ptr<ControlElement> control = this->getControl();
      if (control && control->reactsTo(InputType::MouseInput)) {
        std::cout << "Mouse action executed: Button=" << keyNumber_ << " at time " << this->getActionTime() << "\n";
      }
    }

    void print() const override {
      std::cout << "MouseInput: Button=" << keyNumber_ << ", ActionTime=" << this->getActionTime() << ", ActionType=" << (actionType_ == ActionType::Press ? "Press" : "Release") << "\n";
    }

    MouseInput* clone() const override {
      return new MouseInput(keyNumber_, this->getActionTime(), actionType_, this->getControl());
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

class TouchInput: public InputAction {
  public:
    TouchInput(double actionTime, std::vector<FingerCoordinates> fingersCords, const std::shared_ptr<ControlElement> control)
    : InputAction(actionTime, control){
      fingersCords_ = std::move(fingersCords);
      fingersCount_ = fingersCords_.size();
    }

    void executeAction() const override {
      std::shared_ptr<ControlElement> control = this->getControl();
      if (control && control->reactsTo(InputType::TouchInput)) {
        std::cout << "Touch action executed with " << fingersCount_ << " fingers at time " << this->getActionTime() << "\n";
      }
    }

    void print() const override {
      std::cout << "TouchInput: ActionTime=" << this->getActionTime() << ", FingersCount=" << fingersCount_ << ", Coordinates=";
      for (const auto& coord : fingersCords_) {
        std::cout << " [Start(" << coord.start.first << ", " << coord.start.second << ") -> End(" << coord.end.first << ", " << coord.end.second << ")]";
      }
      std::cout << "\n";
    }

    TouchInput* clone() const override {
      return new TouchInput(this->getActionTime(), fingersCords_, this->getControl());
    }

  private:
    std::vector<FingerCoordinates> fingersCords_;
    int fingersCount_;
};

class ActionsQueue {
  public:
    ActionsQueue(): actions_({}) {}

    ActionsQueue(const ActionsQueue& other) {
      actions_.reserve(other.actions_.size());
      for (const auto& action : other.actions_) {
        actions_.push_back(std::shared_ptr<InputAction>(action->clone()));
      }
    }

    ActionsQueue& operator=(const ActionsQueue& other) {
      if (this != &other) {
        actions_.clear();
        actions_.reserve(other.actions_.size());
        for (const auto& action : other.actions_) {
          actions_.push_back(std::shared_ptr<InputAction>(action->clone()));
        }
      }
      return *this;
    }


    void addAction(std::shared_ptr<InputAction> action) {
      actions_.push_back(action);
      std::sort(actions_.begin(), actions_.end(), [](const std::shared_ptr<InputAction>& a, const std::shared_ptr<InputAction>& b) {
        return *a < *b;
      });
    }

    std::vector<std::shared_ptr<InputAction>> getActionsForControl(const std::string& controlName, double startTime = 0.0, double endTime = std::numeric_limits<double>::max()) const {
      std::vector<std::shared_ptr<InputAction>> result;
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
    std::vector<std::shared_ptr<InputAction>> actions_;
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
  KeyboardInput keyPress("A", 1.0, ActionType::Press, std::make_shared<ControlElement>(keyboardControl));
  KeyboardInput keyRelease = KeyboardInput::createRelease(keyPress, 2.0);
  assert(keyPress.getControl()->getName() == "KeyboardControl");
  assert(keyPress.getActionTime() == 1.0);
  assert(keyRelease.getActionTime() == 2.0);

  // Test MouseInput
  ControlElement mouseControl("MouseControl", {InputType::MouseInput});
  MouseInput mousePress(1, 1.5, ActionType::Press, std::make_shared<ControlElement>(mouseControl));
  assert(mousePress.getControl()->getName() == "MouseControl");
  assert(mousePress.getActionTime() == 1.5);

  // Test TouchInput
  ControlElement touchControl("TouchControl", {InputType::TouchInput});
  std::vector<FingerCoordinates> fingers = {
      FingerCoordinates(0.0, 0.0, 1.0, 1.0),
      FingerCoordinates(1.0, 1.0, 2.0, 2.0)
  };
  TouchInput touch(2.5, fingers, std::make_shared<ControlElement>(touchControl));
  assert(touch.getControl()->getName() == "TouchControl");
  assert(touch.getActionTime() == 2.5);

  // Test polymorphism
  std::vector<std::shared_ptr<InputAction>> inputs;
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

  std::vector<std::shared_ptr<InputAction>> retrievedActions = actionQueue.getActionsForControl("MouseControl", 0.0, 2.0);
  assert(retrievedActions.size() == 1);
  assert(retrievedActions[0]->getActionTime() == 1.5);

  actionQueue.executeActions();

  std::cout << "All tests passed!" << std::endl;

  return 0;
}
