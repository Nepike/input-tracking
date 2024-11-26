#include <iostream>
#include <string>

using namespace std;

class Base {
public:
    virtual ~Base() = default;
};

class Child : public Base {
public:
};

class Child2 : public Base {
public:
};

void checkType(Base* ptr) {
    if (dynamic_cast<Child*>(ptr)) {
        cout << "Object is a Child" << endl;
    } else if (dynamic_cast<Child2*>(ptr)) {
        cout << "Object is a Child2" << endl;
    } else if (dynamic_cast<Base*>(ptr)) {
        cout << "Object is a Base" << endl;
    } else {
        cout << "Unknown type" << endl; //NEVER HAPPENS
    }
}

int main() {
    Base* b = new Base;
    Base* c = new Child;
    Base* c2 = new Child2;

    checkType(b);
    checkType(c);
    checkType(c2);

    delete b;
    delete c;
    delete c2;

    return 0;
}
