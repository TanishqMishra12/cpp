#include <iostream>
using namespace std;

class demo {
    int x;
public:
    demo(int a) : x(a) {}
    friend void display(demo);
};

void display(demo d1) {
    cout << d1.x << endl;
}

class sample {
    int a;
    int b;
public:
    void setval() { a = 25; b = 40; }
    friend float mean(sample s);
};

float mean(sample s) {
    return (s.a + s.b) / 2.0;
}

int main() {
    demo d2(5);
    display(d2);

    sample X;
    X.setval();
    cout << mean(X) << endl;

    return 0;
}