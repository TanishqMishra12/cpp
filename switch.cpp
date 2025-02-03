#include <iostream>
using namespace std;
int main(){
    char grade;
    cout << "Enter your grade: ";
    cin >> grade;
    switch (grade) {
        case 'A':
            cout<<"The grade is 4.0"<<endl;
            break;
        case 'B' :
            cout<<"The grade is 3.0"<<endl;
            break;
        case 'C' :
            cout<<"The grade is 2.0"<<endl;
            break;
        case 'D' :
            cout<<"The grade is 1.0"<<endl;
            break;
        case 'F' :
            cout<<"The grade is 0.0"<<endl;
            break;
        default:
            cout<<"Enter a valid grade"<<endl;
    }
    return 0;
