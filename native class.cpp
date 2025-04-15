#include <iostream>
using namespace std;

void PrintInt(int n) {
    cout << "Value is " << n << endl;
}

void PrintChar(char ch) {
    cout << "Value is " << ch << endl;
}

void PrintFloat(float x) {
    cout << "Value is " << x << endl;
}

void PrintDouble(double d) {
    cout << "Value is " << d << endl;
}

int main() {
    PrintInt(5);
    PrintChar('A');
    PrintFloat(3.14f);
    PrintDouble(2.718);
    return 0;
}                                                   