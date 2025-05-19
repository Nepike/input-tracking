#include "InputManager.hpp"
#include <cassert>
#include <iostream>

int main() {
    InputManager inputManager;

  inputManager.addControlElement("Textbox", {InputType::KEYBOARD});
  inputManager.addControlElement("Button1", {InputType::MOUSE});
  inputManager.addControlElement("Touchscreen", {InputType::TOUCH});

  assert(inputManager.getControlElement("Textbox") != nullptr);
  assert(inputManager.getControlElement("Button1") != nullptr);
  assert(inputManager.getControlElement("Touchscreen") != nullptr);
  assert(inputManager.getControlElement("NonExistent") == nullptr);

  inputManager.addKeyboardAction("Space", 1.5, ActionType::PRESS, "Textbox");
  inputManager.addMouseAction(1, 2.0, ActionType::PRESS, "Button1");
  inputManager.addTouchAction(3.0, {{0, 0, 100, 100}}, "Touchscreen");

  auto actionsJump = inputManager.getControlElement("Textbox");
  auto actionsShoot = inputManager.getControlElement("Button1");
  auto actionsSwipe = inputManager.getControlElement("Touchscreen");

  assert(actionsJump->checkAction(InputType::KEYBOARD));
  assert(actionsShoot->checkAction(InputType::MOUSE));
  assert(actionsSwipe->checkAction(InputType::TOUCH));

  inputManager.performActions(10);

  std::cout << "All tests passed!\n";
  return 0;
}
