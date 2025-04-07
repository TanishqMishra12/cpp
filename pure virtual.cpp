#include <iostream>

class Base {
public:
    virtual void foo() = 0; // Pure virtual function
};

class Derived : public Base {
public:
    void foo() override {
        std::cout << "Derived::Foo" << std::endl;
    }
};

int main() {
    
    Derived derived;
    derived.foo(); 

    Base* basePointer = &derived;
    basePointer->foo(); 

    return 0;
}

