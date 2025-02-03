#include <iostream>
using namespace std;
int main() {
    int num1;
    int num2;
    cout << "Enter two numbers: ";
    cout << "Enter first number: ";
    cout << "Enter second number: ";
    cin >> num1 >> num2;

    if (num1==num2)
    {
        cout << "Both numbers are equal.";
    }
    if (num1!=num2)
    {
        cout << "Both numbers are not equal.";
    }
    
    if(num1 > num2)
    {
        cout << "First number is greater than second number.";
    }
    else if(num2 > num1)
    {
        cout << "Second number is greater than first number.";
    }
    else
    {
        cout << "Both numbers are equal.";
    }
return 0;
}