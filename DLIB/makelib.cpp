#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "ControlElement.hpp"
#include "InputAction.hpp"
#include "LeftistHeap.hpp"
#include "ActionsQueue.hpp"
#include "InputManager.hpp"

namespace py = pybind11;

PYBIND11_MODULE(inputs_module, m) {
    m.doc() = "Input-tracking module via pybind11";

    // enum InputType
    py::enum_<InputType>(m, "InputType")
        .value("KEYBOARD", InputType::KEYBOARD)
        .value("MOUSE",    InputType::MOUSE)
        .value("TOUCH",    InputType::TOUCH)
        .export_values();

    // enum ActionType
    py::enum_<ActionType>(m, "ActionType")
        .value("PRESS",   ActionType::PRESS)
        .value("RELEASE", ActionType::RELEASE)
        .export_values();

    // FingerCoordinates
    py::class_<FingerCoordinates>(m, "FingerCoordinates")
        .def(py::init<double, double, double, double>(),
             py::arg("sx"), py::arg("sy"),
             py::arg("ex"), py::arg("ey"))
        .def_readwrite("sx", &FingerCoordinates::sx)
        .def_readwrite("sy", &FingerCoordinates::sy)
        .def_readwrite("ex", &FingerCoordinates::ex)
        .def_readwrite("ey", &FingerCoordinates::ey);

    // ControlElement
    py::class_<ControlElement, std::shared_ptr<ControlElement>>(m, "ControlElement")
        .def(py::init<const std::string&, const std::unordered_set<InputType>&>(),
             py::arg("name"), py::arg("allowed_actions"))
        .def("addAction",    &ControlElement::addAction)
        .def("removeAction", &ControlElement::removeAction)
        .def("checkAction",  &ControlElement::checkAction)
        .def("getName",      &ControlElement::getName);

    // InputManager
    py::class_<InputManager>(m, "InputManager")
        .def(py::init<>())
        .def("addControlElement",
            [](InputManager &self,
               const std::string &name,
               const std::unordered_set<InputType> &allowed) {
                self.addControlElement(name, allowed);
            },
            py::arg("name"), py::arg("allowed_actions"))
        .def("getControlElement",
            &InputManager::getControlElement,
            py::arg("name"))
        .def("addKeyboardAction",
             &InputManager::addKeyboardAction,
             py::arg("key"), py::arg("time"),
             py::arg("action_type"), py::arg("control_name"))
        .def("addMouseAction",
             &InputManager::addMouseAction,
             py::arg("button"), py::arg("time"),
             py::arg("action_type"), py::arg("control_name"))
        .def("addTouchAction",
             &InputManager::addTouchAction,
             py::arg("time"), py::arg("finger_coords"),
             py::arg("control_name"))
        .def("performActions",
             &InputManager::performActions,
             py::arg("current_time"));
}


/*
g++ -O3 -Wall -shared -std=c++17 -fPIC -I ".\DLIB\.venv\Lib\site-packages\pybind11\include" -I "C:\Users\maxve\AppData\Local\Programs\Python\Python311\include" -I "APP\" ./APP/*.cpp ./DLIB/makelib.cpp -LC:\Users\maxve\AppData\Local\Programs\Python\Python311\libs -lpython311 -o ./DLIB/inputs_module.pyd; cp ./DLIB/inputs_module.pyd .\DLIB\.venv\Lib\site-packages\
*/
