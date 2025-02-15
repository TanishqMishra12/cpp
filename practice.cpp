/*#include <iostream>
using namespace std;
int main(){
    double x = 10;
    double y = 5;
    double z = (x+10)/(3*y);
    cout << z;
    return 0;
}
#include <iostream>
using namespace std;
int main() {
    int sales = 95000;
    double statetax = 0.04*sales;
    double countytax = 0.02*sales;
    double totaltax = statetax + countytax;
    cout << "sales = "<< sales<< endl;
    cout <<"statetax = "<< statetax<< endl;
    cout << "countytax = "<< countytax<< endl;
    cout <<"totaltax = "<< totaltax<< endl;
    return 0;
}
#include <iostream>
using namespace std;
int main() {
    cout << "enter the temperature in celsius: ";
    double celsius;
    cin >> celsius;
    double fahrenheit = (celsius * 9/5) + 32;
    cout << "the temperature in fahrenheit is: " << fahrenheit << endl;
    return 0;
}
#include <iostream>
#include <cmath>
using namespace std;
int main() {
    double result;
     result = floor (45.677);
     result = ceil (45.677);
     result = sqrt (45.677);
     result = pow (45.677, 2);
    cout << "Floor: " << floor(45.677) << endl;
    cout << "Ceil: " << ceil(45.677) << endl;
    cout << "Sqrt: " << sqrt(45.677) << endl;
    cout << "Pow: " << pow(45.677, 2) << endl;
    return 0;
}

#include <iostream>
#include <cmath>23
using namespace std;
int main() {
    double radius;
    cout << "Enter the radius of the circle: ";
    cin >> radius;
    cout << "The area of the circle is: " << 3.14159 * pow(radius, 2) << endl;
    return 0;
}

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
int main() {
    srand (time(0));
    int number = rand () %10;
    cout << number;
    return 0;
}*/


#include <iostream>
using namespace std;
#include <cstdlib>
#include <ctime>


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