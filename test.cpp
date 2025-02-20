#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "main.cpp"

namespace py = pybind11;


PYBIND11_MODULE(test_module, m) {
    // Регистрация перечислений
    py::enum_<InputType>(m, "InputType")
        .value("KeyboardInput", InputType::KeyboardInput)
        .value("MouseInput", InputType::MouseInput)
        .value("TouchInput", InputType::TouchInput);

    py::enum_<ActionType>(m, "ActionType")
        .value("Press", ActionType::Press)
        .value("Release", ActionType::Release);

    // Регистрация FingerCoordinates
    py::class_<FingerCoordinates>(m, "FingerCoordinates")
        .def(py::init<double, double, double, double>())
        .def_readwrite("start", &FingerCoordinates::start)
        .def_readwrite("end", &FingerCoordinates::end);

    // Регистрация ControlElement с поддержкой shared_ptr
    py::class_<ControlElement, std::shared_ptr<ControlElement>>(m, "ControlElement")
        .def(py::init<const std::string&, const std::unordered_set<InputType>&>())
        .def("addAction", &ControlElement::addAction)
        .def("removeAction", &ControlElement::removeAction)
        .def("checkAction", &ControlElement::checkAction)
        .def("getName", &ControlElement::getName);

    // Регистрация InputManager
    py::class_<InputManager>(m, "InputManager")
        .def(py::init<>())
        .def("addControlElement", [](InputManager &self, const std::string &name, const py::set &allowed_actions_py) {
            std::unordered_set<InputType> allowed_actions;
            for (const auto &action : allowed_actions_py) {
                allowed_actions.insert(action.cast<InputType>());
            }
            self.addControlElement(name, allowed_actions);
        })
        .def("getControlElement", &InputManager::getControlElement)
        .def("addKeyboardAction", &InputManager::addKeyboardAction)
        .def("addMouseAction", &InputManager::addMouseAction)
        .def("addTouchAction", &InputManager::addTouchAction)
        .def("performActions", &InputManager::performActions);
}

/*
g++ -O3 -Wall -shared -std=c++17 -fPIC -I ".\venv\Lib\site-packages\pybind11\include" -I "C:\Users\maxve\AppData\Local\Programs\Python\Python311\include" test.cpp -LC:\Users\maxve\AppData\Local\Programs\Python\Python311\libs -lpython311 -o test_module.pyd; cp test_module.pyd .\venv\Lib\site-packages\
*/

