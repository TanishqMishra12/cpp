#include <iostream>
#include <cmath>
using namespace std;

class Circle {
private:
    double radius;
public:
    void store(double r);
    double area() const;
    void display() const;
};

// member function definitions
void Circle::store(double r) {
    radius = r;
}

double Circle::area() const {
    return M_PI * radius * radius;
}

void Circle::display() const {
    cout << "r = " << radius << endl;
}
int main(){
    Circle c ; // an object of Circle class
    c.store(5.0);
    cout << "The area of circle c is " << c.area() << endl;
    c.display();
    return 0;
}