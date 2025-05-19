import inputs_module as im
import time


if __name__ == "__main__":
    manager = im.InputManager()
    manager.addControlElement("button1", {im.InputType.KEYBOARD, im.InputType.TOUCH, im.InputType.MOUSE})

    manager.addKeyboardAction("Enter", 11, im.ActionType.PRESS, "button1")
    manager.addKeyboardAction("Shift", 10, im.ActionType.PRESS, "button1")
    manager.addKeyboardAction("F12", 12, im.ActionType.PRESS, "button1")

    manager.addMouseAction(1, 2, im.ActionType.RELEASE, "button1")

    swipe1 = im.FingerCoordinates(1, 1, 2, 2)
    swipe2 = im.FingerCoordinates(3, 3, 2, 2)

    manager.addTouchAction(4, [swipe1, swipe2], "button1")

    for i in range(1, 15):
        responses = manager.performActions(i)
        for r in responses:
            print(f"[TIME:{i}]" + r)
        time.sleep(1)

