#include <iostream>
#include <cmath>  
using namespace std;

class Shape {
   protected:
    float dimension;

   public:
    Shape(float dim = 0) : dimension(dim) {} 

    virtual void getDimension() {
        cin >> dimension;
    }

    // Pure virtual function for area calculation
    virtual float calculateArea() const = 0;

    virtual ~Shape() {}  
};

class Square : public Shape {
   public:
    Square(float dim = 0) : Shape(dim) {}

    float calculateArea() const override {
        return dimension * dimension;
    }
};

class Circle : public Shape {
   public:
    Circle(float dim = 0) : Shape(dim) {}  

    float calculateArea() const override {
        return M_PI * dimension * dimension;  
    }
};

int main() {
    Square s;
    cout << "Enter side of square: ";
    s.getDimension();
    cout << "Area of square: " << s.calculateArea() << endl;

    Circle c;
    cout << "Enter radius of circle: ";
    c.getDimension();
    cout << "Area of circle: " << c.calculateArea() << endl;

    return 0;
}
