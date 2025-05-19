import test_module as input_manager


def test_control_element():
    print("=== Testing ControlElement ===")
    control = input_manager.ControlElement("test_control", {input_manager.InputType.KeyboardInput})
    print(f"Control name: {control.getName()}")  # Ожидаем: test_control
    print(f"Check KeyboardInput: {control.checkAction(input_manager.InputType.KeyboardInput)}")  # Ожидаем: True
    print(f"Check MouseInput: {control.checkAction(input_manager.InputType.MouseInput)}")  # Ожидаем: False

    control.addAction(input_manager.InputType.MouseInput)
    print(f"Check MouseInput after add: {control.checkAction(input_manager.InputType.MouseInput)}")  # Ожидаем: True

    control.removeAction(input_manager.InputType.MouseInput)
    print(f"Check MouseInput after remove: {control.checkAction(input_manager.InputType.MouseInput)}")  # Ожидаем: False
    print()


def test_input_manager():
    print("=== Testing InputManager ===")
    im = input_manager.InputManager()

    # Добавляем элемент управления
    im.addControlElement("keyboard_control", {input_manager.InputType.KeyboardInput})
    im.addControlElement("mouse_control", {input_manager.InputType.MouseInput})
    im.addControlElement("touch_control", {input_manager.InputType.TouchInput})

    # Получаем элемент управления
    keyboard_control = im.getControlElement("keyboard_control")
    if keyboard_control:
        print(f"Found control: {keyboard_control.getName()}")  # Ожидаем: keyboard_control
    else:
        print("Control not found!")

    # Добавляем действия
    im.addKeyboardAction("A", 1.0, input_manager.ActionType.Press, "keyboard_control")
    im.addMouseAction(1, 2.0, input_manager.ActionType.Release, "mouse_control")
    im.addTouchAction(3.0, [input_manager.FingerCoordinates(0.0, 0.0, 1.0, 1.0)], "touch_control")

    # Выполняем действия
    print("Performing actions at time 0.5:")
    im.performActions(0.5)  # Ничего не должно выполниться
    print("Performing actions at time 1.5:")
    im.performActions(1.5)  # Должно выполниться действие клавиатуры
    print("Performing actions at time 2.5:")
    im.performActions(2.5)  # Должно выполниться действие мыши
    print("Performing actions at time 3.5:")
    im.performActions(3.5)  # Должно выполниться действие touch
    print()


def test_actions_queue():
    print("=== Testing ActionsQueue ===")
    im = input_manager.InputManager()
    im.addControlElement("test_control", {input_manager.InputType.KeyboardInput})

    # Добавляем несколько действий
    im.addKeyboardAction("A", 1.0, input_manager.ActionType.Press, "test_control")
    im.addKeyboardAction("B", 2.0, input_manager.ActionType.Press, "test_control")
    im.addKeyboardAction("C", 3.0, input_manager.ActionType.Press, "test_control")

    # Выполняем действия
    print("Performing actions at time 1.5:")
    im.performActions(1.5)  # Должно выполниться первое действие
    print("Performing actions at time 2.5:")
    im.performActions(2.5)  # Должно выполниться второе действие
    print("Performing actions at time 3.5:")
    im.performActions(3.5)  # Должно выполниться третье действие
    print()


def test_finger_coordinates():
    print("=== Testing FingerCoordinates ===")
    finger = input_manager.FingerCoordinates(0.0, 0.0, 1.0, 1.0)
    print(f"Start: {finger.start}")  # Ожидаем: (0.0, 0.0)
    print(f"End: {finger.end}")  # Ожидаем: (1.0, 1.0)
    print()


if __name__ == "__main__":
    test_control_element()
    test_input_manager()
    test_actions_queue()
    test_finger_coordinates()

