#include <iostream>
using namespace std;

void Print(int n) {
    cout << "*** Debug" << endl;
    cout << "Value is " << n << endl;
}

void Print(float x) {
    cout << "Value is " << x << endl;
}

int main() {
    Print(10);
    Print(5.75f);
    return 0;
}