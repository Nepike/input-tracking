#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <memory>
#include <unordered_map>


enum class InputType {
  KeyboardInput,
  MouseInput,
  TouchInput,
};

// Возможно, вместо енама лучше использовать булевое поле is_press ?
enum class ActionType {
  Press,
  Release,
};


class ControlElement {
  public:
    ControlElement(const std::string& name, const std::unordered_set<InputType>& allowedActions) : name_(name), allowedActions_(allowedActions) {}

    void addAction(InputType action) {
      allowedActions_.insert(action);
    }

    void removeAction(InputType action) {
      allowedActions_.erase(action);
    }

    bool checkAction(InputType input) const {
      return allowedActions_.find(input) != allowedActions_.end();
    }

    const std::string& getName() const {
      return name_;
    }

  private:
    std::string name_;
    std::unordered_set<InputType> allowedActions_; 
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

    virtual bool performAction() const = 0;

    virtual InputAction* clone() const = 0;

  private:
    double actionTime_;
    // Один и тот же элемент управления будет целью множества действий. Кажется, нужно использовать shared
    const std::shared_ptr<ControlElement> controlElement_;
};


class KeyboardInput: public InputAction {
  public:
    KeyboardInput(const std::string& keyName, double actionTime, ActionType actionType, const std::shared_ptr<ControlElement>& control)
    : InputAction(actionTime, control), keyName_(keyName), actionType_(actionType) {}

    static KeyboardInput createRelease(const KeyboardInput& pressAction, double releaunordered_setime){
      return KeyboardInput(pressAction.keyName_, releaunordered_setime, ActionType::Release, pressAction.getControl());
    }

    bool performAction() const override {
      std::shared_ptr<ControlElement> control = this->getControl();
      if (control && control->checkAction(InputType::KeyboardInput)) {
        // *Действие выполнено*
        return true;
      }
      return false;
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
    : InputAction(actionTime, control), keyNumber_(keyNumber), actionType_(actionType) {}

    bool performAction() const override {
      std::shared_ptr<ControlElement> control = this->getControl();
      if (control && control->checkAction(InputType::MouseInput)) {
        // *Действие выполнено*
        return true;
      }
      return false;
    }

    MouseInput* clone() const override {
      return new MouseInput(keyNumber_, this->getActionTime(), actionType_, this->getControl());
    }

  private:
    int keyNumber_;
    ActionType actionType_;
};


// Вспомогательная структура
struct FingerCoordinates {
  FingerCoordinates(double sx, double sy, double ex, double ey) : start{sx, sy}, end{ex, ey} {}
  std::pair<double, double> start;
  std::pair<double, double> end;
};


class TouchInput: public InputAction {
  public:
    TouchInput(double actionTime, std::vector<FingerCoordinates> fingersCords, const std::shared_ptr<ControlElement>& control)
    : InputAction(actionTime, control), fingersCords_(std::move(fingersCords)){}

    bool performAction() const override {
      std::shared_ptr<ControlElement> control = this->getControl();
      if (control && control->checkAction(InputType::TouchInput)) {
        // *Действие выполнено*
        return true;
      }
      return false;
    }

    TouchInput* clone() const override {
      return new TouchInput(this->getActionTime(), fingersCords_, this->getControl());
    }

  private:
    std::vector<FingerCoordinates> fingersCords_;
    // int fingersCount_ совпадает с fingersCords_.size()
};


class ActionsQueue {
  public:
    ActionsQueue() = default;

    ActionsQueue(const ActionsQueue& other) {
      actions_.reserve(other.actions_.size());
      for (const auto& action : other.actions_) {
        actions_.push_back(std::unique_ptr<InputAction>(action->clone()));
      }
    }

    ActionsQueue& operator=(const ActionsQueue& other) {
      if (this != &other) {
        actions_.clear();
        actions_.reserve(other.actions_.size());
        for (const auto& action : other.actions_) {
          actions_.push_back(std::unique_ptr<InputAction>(action->clone()));
        }
      }
      return *this;
    }

    void addAction(std::unique_ptr<InputAction> action) {
      actions_.push_back(std::move(action));
      // Возможно стоит использовать бинпоиск для вставки в нужную позицию вместо сортировки после каждого добавления.
      std::sort(actions_.begin(), actions_.end(), [](const auto& a, const auto& b) {
        return *a < *b;
      });
    }

    std::vector<InputAction*> getActionsForControl(const std::string& controlName, double startTime = 0.0, double endTime = std::numeric_limits<double>::max()) const {
      std::vector<InputAction*> result;
      for (const auto& action : actions_) {
        if (action->getControl()->getName() == controlName && action->getActionTime() >= startTime && action->getActionTime() <= endTime) {
          result.push_back(action.get());
        }
      }
      return result;
    }

    void performActions() {
      // TODO Добавить в performAction проверку, чтобы действие не могло выполниться пока не пришло его время

      // Задумка - удалять из очереди выполненые действия, чтобы можно было запускать функицию каждую секунду.
      auto it = actions_.begin();
      while (it != actions_.end()) {
        if ((*it)->performAction()) {
          it = actions_.erase(it);
        }
        else {++it;}
      }
    }

  private:
    // По задумке очередь - единственный владелец действий, наверное, правильно использовать unique_ptr ?
    std::vector<std::unique_ptr<InputAction>> actions_;
};


// Фасад
class InputManager {
  public:
    InputManager() = default;

    void addControlElement(const std::string& name, const std::unordered_set<InputType>& allowedActions) {
      controls_[name] = std::make_shared<ControlElement>(name, allowedActions);
    }

    std::shared_ptr<ControlElement> getControlElement(const std::string& name) const {
      auto it = controls_.find(name);
      return (it != controls_.end()) ? it->second : nullptr;
    }

    void addKeyboardAction(const std::string& key, double time, ActionType actionType, const std::string& controlName) {
      auto control = getControlElement(controlName);
      if (control) {
        actionsQueue_.addAction(std::make_unique<KeyboardInput>(key, time, actionType, control));
      }
    }

    void addMouseAction(int keyNumber, double time, ActionType actionType, const std::string& controlName) {
      auto control = getControlElement(controlName);
      if (control) {
        actionsQueue_.addAction(std::make_unique<MouseInput>(keyNumber, time, actionType, control));
      }
    }

    void addTouchAction(double time, std::vector<FingerCoordinates> fingers, const std::string& controlName) {
      auto control = getControlElement(controlName);
      if (control) {
        actionsQueue_.addAction(std::make_unique<TouchInput>(time, std::move(fingers), control));
      }
    }

    void performActions() {
      actionsQueue_.performActions();
    }

  private:
    std::unordered_map<std::string, std::shared_ptr<ControlElement>> controls_;
    ActionsQueue actionsQueue_;
};


int main() {
  // // Тест ControlElement
  // ControlElement button("Jump", {InputType::KeyboardInput});
  // assert(button.getName() == "Jump");
  // assert(button.checkAction(InputType::KeyboardInput) == true);
  // assert(button.checkAction(InputType::MouseInput) == false);
  // button.addAction(InputType::MouseInput);
  // assert(button.checkAction(InputType::MouseInput) == true);
  // button.removeAction(InputType::KeyboardInput);
  // assert(button.checkAction(InputType::KeyboardInput) == false);

  // // Тест KeyboardInput
  // auto control = std::make_shared<ControlElement>("Jump", std::unordered_set<InputType>{InputType::KeyboardInput});
  // KeyboardInput keyPress("Space", 1.0, ActionType::Press, control);
  // assert(keyPress.getActionTime() == 1.0);
  // assert(keyPress.performAction() == true);
  // auto keyRelease = KeyboardInput::createRelease(keyPress, 2.0);
  // assert(keyRelease.getActionTime() == 2.0);
  // assert(keyRelease.performAction() == true);

  // // Тест MouseInput
  // auto mouseControl = std::make_shared<ControlElement>("Fire", std::unordered_set<InputType>{InputType::MouseInput});
  // MouseInput mouseClick(1, 1.5, ActionType::Press, mouseControl);
  // assert(mouseClick.getActionTime() == 1.5);
  // assert(mouseClick.performAction() == true);

  // // Тест TouchInput
  // auto touchControl = std::make_shared<ControlElement>("Swipe", std::unordered_set<InputType>{InputType::TouchInput});
  // TouchInput touchAction(3.0, {{0.0, 0.0, 1.0, 1.0}}, touchControl);
  // assert(touchAction.getActionTime() == 3.0);
  // assert(touchAction.performAction() == true);

  // // Тест ActionsQueue
  // ActionsQueue queue;
  // queue.addAction(std::make_unique<KeyboardInput>("Enter", 2.5, ActionType::Press, control));
  // queue.addAction(std::make_unique<MouseInput>(2, 3.0, ActionType::Release, mouseControl));
  // auto actions = queue.getActionsForControl("Jump");
  // assert(actions.size() == 1);
  // assert(actions[0]->getActionTime() == 2.5);

  // std::cout << "All tests passed successfully!" << std::endl;

  InputManager inputManager;

  inputManager.addControlElement("Textbox", {InputType::KeyboardInput});
  inputManager.addControlElement("Button1", {InputType::MouseInput});
  inputManager.addControlElement("Touchscreen", {InputType::TouchInput});

  assert(inputManager.getControlElement("Textbox") != nullptr);
  assert(inputManager.getControlElement("Button1") != nullptr);
  assert(inputManager.getControlElement("Touchscreen") != nullptr);
  assert(inputManager.getControlElement("NonExistent") == nullptr);

  inputManager.addKeyboardAction("Space", 1.5, ActionType::Press, "Textbox");
  inputManager.addMouseAction(1, 2.0, ActionType::Press, "Button1");
  inputManager.addTouchAction(3.0, {FingerCoordinates(0, 0, 100, 100)}, "Touchscreen");

  auto actionsJump = inputManager.getControlElement("Textbox");
  auto actionsShoot = inputManager.getControlElement("Button1");
  auto actionsSwipe = inputManager.getControlElement("Touchscreen");

  assert(actionsJump->checkAction(InputType::KeyboardInput));
  assert(actionsShoot->checkAction(InputType::MouseInput));
  assert(actionsSwipe->checkAction(InputType::TouchInput));

  inputManager.performActions();

  std::cout << "All tests passed!\n";
  return 0;
}
