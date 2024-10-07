#include <iostream>
#include <string>
#include <cassert>

enum class ActionType {
    Press,
    Release
};

class InputAction {
public:
    virtual ~InputAction() = default; // на всякий случай
    virtual std::string getType() const = 0;
protected:
    // smth
private:
    // smth
};


class KeyboardInput: public InputAction {
public:
    KeyboardInput(const std::string& keyName, double actionTime, ActionType actionType, const std::string& controlName)
        : keyName_(keyName), actionTime_(actionTime), actionType_(actionType), controlName_(controlName) {}

    //по поводу const string& - Я читал, что это может быть менее эффективно из-за Named Return Value Optimization (NRVO)
    std::string getType() const override {
        return "KeyboardInput";
    }
    // const std::string& getType() const override {
    //     static const std::string type = "KeyboardInput";
    //     return type;
    // }


    std::string show() const {
        return "keyname: " + keyName_ + "\n" +
                "actionTime: " + std::to_string(actionTime_) + "\n" +
                "actionType: " + (actionType_ == ActionType::Press ? "press" : "release") + "\n" +
                "controlName: " + controlName_ + "\n";
    }

    // Создание объекта "отпускание" по объекту "нажатие" и времени отпускания
    static KeyboardInput createRelease(const KeyboardInput& pressAction, double releaseTime) {
        return KeyboardInput(pressAction.keyName_, releaseTime, ActionType::Release, pressAction.controlName_);
    }

    bool operator<(const KeyboardInput& other) const {
        return actionTime_ < other.actionTime_;
    }

    bool operator==(const KeyboardInput& other) const {
        return actionTime_ == other.actionTime_;
    }
    
    
private:
    std::string keyName_;
    double actionTime_; // Это не длительность, а именно время!
    ActionType actionType_;
    std::string controlName_;
};


int main() {
    std::cout << "Testing KeyboardInput!\n";

    KeyboardInput press_shift = KeyboardInput("SHIFT", 123.321, ActionType::Press, "some_ctrl");
    KeyboardInput release_shift = KeyboardInput::createRelease(press_shift, 321.123);

    assert(press_shift < release_shift);
    assert(!(press_shift == release_shift));

    // Тестирование полиморфизма
    InputAction* ptr = &press_shift;
    assert(ptr->getType() == "KeyboardInput");

    std::cout << "All tests passed successfully!\n\n";

    std::cout << press_shift.show() << std::endl << release_shift.show() << std::endl;

    return 0;
}
