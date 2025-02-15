#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    const short minvalue = 1;
    const short maxvalue = 6;
    srand(time(0));
    short number = rand() % (maxvalue - minvalue + 1) + minvalue;
    short number2 = rand() % (maxvalue - minvalue + 1) + minvalue;
    cout << "The first number is: " << number << endl;
    cout << "The second number is: " << number2 << endl;
    return 0;
}