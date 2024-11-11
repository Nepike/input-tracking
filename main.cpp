#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <set>
#include <utility>


enum InputType {
  KeyboardInput,
  MouseInput,
  TouchInput,
};

enum ActionType {
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

    const std::string& getControlName() const {
      return controlElement_->getName();
    }

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


int main() {
    return 0;
}




