#include <iostream>
#include <string>
#include <cassert>



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
    KeyboardInput(const std::string& keyName, double actionTime, const std::string& actionType, const std::string& controlName)
    : keyName_(keyName), actionTime_(actionTime), actionType_(actionType), controlName_(controlName){}

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
                "actionType: " + actionType_ + "\n" +
                "controlName: " + controlName_ + "\n";
    }

    // Создание объекта "отпускание" по объекту "нажатие" и времени отпускания
    static KeyboardInput createRelease(const KeyboardInput& pressAction, double releaseTime) {
        return KeyboardInput(pressAction.keyName_, releaseTime, "release", pressAction.controlName_);
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
    std::string actionType_; // "press" или "release" (возможно стоит вспомнить как работают enum'ы)
    std::string controlName_;
};


int main() {
    std::cout << "Testing KeyboardInput!\n\n";

    KeyboardInput press_shift = KeyboardInput("SHIFT", 123.321, "press", "some_ctrl");
    KeyboardInput release_shift = KeyboardInput::createRelease(press_shift, 321.123);

    assert(press_shift < release_shift);
    assert(!(press_shift == release_shift));

    // Тестирование полиморфизма
    InputAction* ptr = &press_shift;
    assert(ptr->getType() == "KeyboardInput");

    std::cout << "All tests passed successfully!\n";

    return 0;
}
