#include <iostream>
using namespace std;
int main() {
    int x = 5;
    int* ptr1;
    ptr1 = &x;

    int* ptr2;
    ptr2 = ptr1;
    *ptr1 = 6;

    cout << ptr1 << endl;
    cout << *ptr2 << endl;

    return 0;
}

int main() {
    int* ptr3 = new int;
    *ptr3 = 5;
    delete ptr3;
    ptr3 = NULL;

    int *ptr4 = new int[5];
    ptr4[0] = 100;
    ptr4[4] = 123;
    delete[] ptr4;
    ptr4 = NULL;

    return 0;
}

void increment(int b1, int &b2, int *b3) {
    b1 += 2;
    b2 += 2;
    *b3 += 2;
}

int main() {
    int a1 = 5, a2 = 10;
    int *a3 = new int;
    *a3 = 15;
    int &a4 = *a3;

    cout << a1 << " " << a2 << " " << *a3 << endl;
    increment(a1, a2, a3);
    cout << a1 << " " << a2 << " " << *a3 << endl;

    delete a3;
    a3 = NULL;
    return 0;
}