#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>


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
    virtual InputType getType() const = 0;
    
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

    // TODO
    // void executeAction(){}
    // void print(){}

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

    InputType getType() const override {
      return InputType::KeyboardInput;
    }

    static KeyboardInput createRelease(const KeyboardInput& pressAction, double releaseTime){
      return KeyboardInput(pressAction.keyName_, releaseTime, ActionType::Release, pressAction.controlElement_);
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

    InputType getType() const override {
      return InputType::MouseInput;
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

    InputType getType() const override {
      return InputType::TouchInput;
    }

  private:
    std::vector<FingerCoordinates> fingersCords_;
    int fingersCount_;
};

class ActionsQueue {
  public:
    ActionsQueue(): actions_({}) {}

    void addAction(Input* action) {
      actions_.push_back(action);
      std::sort(actions_.begin(), actions_.end(), [](const Input* a, const Input* b) {
        return a->getActionTime() < b->getActionTime();
      });
    }

    std::vector<Input*> getActionsForControl(const std::string& controlName, double startTime = 0.0, double endTime = std::numeric_limits<double>::max()) const {
      std::vector<Input*> result;
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
        const ControlElement* control = action->getControl();
        if (control->reactsTo(action->getType())){
          std::cout << "Executing action on control '" << control->getName() << "' at time " << action->getActionTime() << std::endl;
        }
      }
    }

  private:
    std::vector<Input*> actions_;
};



int main() {
  ControlElement btn1("button1", {InputType::KeyboardInput, InputType::MouseInput});
  ControlElement btn2("button2", {InputType::KeyboardInput, InputType::MouseInput, InputType::TouchInput});
  Input* keyPress = new KeyboardInput("A", 1.0, ActionType::Release, &btn1);
  Input* keyRelease = new KeyboardInput("A", 2.0, ActionType::Press, &btn1);
  Input* mousePress = new MouseInput(1, 3.0, ActionType::Press, &btn2);
  Input* touchGesture = new TouchInput(6.0, std::vector<FingerCoordinates>({FingerCoordinates(0, 0, 1, 1)}), &btn2);

  ActionsQueue actionQueue;
  actionQueue.addAction(keyPress);
  actionQueue.addAction(keyRelease);
  actionQueue.addAction(mousePress);
  actionQueue.addAction(touchGesture);

  auto actions = actionQueue.getActionsForControl("button1", 1.0, 5.0);
  for (const auto& action : actions) {
    std::cout << "Action on control " << action->getControl()->getName() << " at time " << action->getActionTime() << '\n';
  }

  actionQueue.executeActions();

  return 0;
}




