#include <iostream>
using namespace std;

class Parent {
public:
    void tanishq_Print()
    {
        cout << "Base Function" << endl;
    }
};

class Child : public Parent {
public:
    void tanishq_Print()
    {
        cout << "Derived Function" << endl;
    }
};

int main()
{
    Child Child_Derived;
    Child_Derived.tanishq_Print();
    return 0;
}