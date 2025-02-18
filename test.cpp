#include <pybind11/pybind11.h>

using namespace std;


int add(int a, int b) {
    return a + b + 100;
}

PYBIND11_MODULE(test_module, m) {
    m.def("add", &add, "A function that adds two numbers");
}

/*
g++ -O3 -Wall -shared -std=c++17 -fPIC -I ".\venv\Lib\site-packages\pybind11\include" -I "C:\Users\maxve\AppData\Local\Programs\Python\Python311\include" test.cpp -LC:\Users\maxve\AppData\Local\Programs\Python\Python311\libs -lpython311 -o test_module.pyd; cp test_module.pyd .\venv\Lib\site-packages\
*/

