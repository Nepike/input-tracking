#include <iostream>
#include <string>

using std::string;

// Пока асбтрактный
class InputAction {
public:
    virtual ~InputAction() = default; // на всякий случай
    virtual string getType() const = 0;
protected:
    // smth
private:
    // smth
};


class KeyboardInput: public InputAction {
public:
    KeyboardInput(const string& keyName, double actionTime, const string& actionType, const std::string& controlName)
    : keyName_(keyName), actionTime_(actionTime), actionType_(actionType), controlName_(controlName){}

    string getType() const override {
        return "KeyboardInput";
    }

    void show() const {
        std::cout<< "keyName: " << keyName_ << 
                    "\nactionTime: " << actionTime_ <<
                    "\nactionTipe: " << actionType_ <<
                    "\ncontrolName: " << controlName_;
    }

    // Создание объекта "отпускание" по объекту "нажатие" и времени отпускания
    static KeyboardInput createRelease(const KeyboardInput& pressAction, double releaseTime) {
        return KeyboardInput(pressAction.keyName_, releaseTime, "release", pressAction.controlName_);
    }

    bool operator<(const KeyboardInput& other) const {
        // стоит ли добавить проверкку на равенство keyName? (keyName_ == other.keyName_)
        return actionTime_ < other.actionTime_;
    }

    bool operator==(const KeyboardInput& other) const {
        // стоит ли добавить проверкку на равенство keyName? (keyName_ == other.keyName_)
        return actionTime_ == other.actionTime_;
    }

    //пока обойдемся без spaceship
    
private:
    string keyName_;
    double actionTime_; // Это не длительность, а именно время!
    string actionType_; // "press" или "release" (возможно стоит вспомнить как работают enum'ы)
    string controlName_;
};


int main(){
    std::cout << "Testing KeyboardInput!\n\n";
    KeyboardInput press_shift = KeyboardInput("SHIFT", 123.321, "press", "some_ctrl");
    KeyboardInput release_shift = KeyboardInput::createRelease(press_shift, 321.123);

    press_shift.show();
    std::cout << "\n\n";
    release_shift.show();
    std::cout << "\n\npress_shift < release_shift: " << (press_shift < release_shift);
    std::cout << "\npress_shift == release_shift: " << (press_shift == release_shift);
    

    // полиморфизм пока никак не проверить
    InputAction* ptr = &press_shift;

    return 0;
}